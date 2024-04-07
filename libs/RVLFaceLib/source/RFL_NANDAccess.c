#include <RVLFaceLibInternal.h>
#include <revolution/mem/expHeap.h>
#include <revolution/os.h>
#include <cstdio>

#define ACC_SAFE_BUFFER_SIZE 0x2000
#define MAX_RETRY_COUNT 30

static const char* scFileNames[RFLiFileType_Max] = {
    "/shared2/menu/FaceLib/RFL_DB.dat", //!< RFLiFileType_Database
    "/shared2/menu/FaceLib/RFL_Res.dat" //!< RFLiFileType_Resource
};

static const char* scFirstDirectory = "/shared2/menu";
static const char* scSecondDirectory = "/shared2/menu/FaceLib";

static const u8 scFilePermissions[RFLiFileType_Max] = {
    NAND_PERM_RWALL, // RFLiFileType_Database
    NAND_PERM_RALL   // RFLiFileType_Resource
};

static const u8 scFileAttributes[RFLiFileType_Max] = {
    0, //!< RFLiFileType_Database
    0  //!< RFLiFileType_Resource
};

static void opencallback_(s32 result, NANDCommandBlock* block);
static void createcallback_(s32 result, NANDCommandBlock* block);
static void close2opencallback_(s32 result, NANDCommandBlock* block);
static void readcallback_(s32 result, NANDCommandBlock* block);
static void writecallback_(s32 result, NANDCommandBlock* block);
static void readseekcallback_(s32 result, NANDCommandBlock* block);
static void writecallback_(s32 result, NANDCommandBlock* block);
static void writeseekcallback_(s32 result, NANDCommandBlock* block);
static void closecallback_(s32 result, NANDCommandBlock* block);
static void getlengthcallback_(s32 result, NANDCommandBlock* block);
static void deletecallback_(s32 result, NANDCommandBlock* block);
static void createdircallback1_(s32 result, NANDCommandBlock* block);
static void createdircallback2_(s32 result,
                                NANDCommandBlock* block) NO_INLINE;

void RFLiInitAccessInfo(MEMiHeapHead* heap) {
    u16 i;

    for (i = 0; i < RFLiFileType_Max; i++) {
        RFLiAccessInfo* info = RFLiGetAccInfo((RFLiFileType)i);
        memset(info, 0, sizeof(RFLiAccessInfo));
        info->safeBuffer =
            MEMAllocFromExpHeapEx(heap, ACC_SAFE_BUFFER_SIZE, 32);
        OSCreateAlarm(&info->alarm);
    }
}

void RFLiExitAccessInfo(void) {
    u16 i;

    for (i = 0; i < RFLiFileType_Max; i++) {
        RFLiAccessInfo* info = RFLiGetAccInfo((RFLiFileType)i);
        OSCancelAlarm(&info->alarm);
    }
}

BOOL RFLiIsWorking(void) {
    return RFLiGetWorking();
}

void RFLiStartWorking(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    RFLiSetWorking(TRUE);
    RFLiGetManager()->lastErrCode = RFLErrcode_Busy;

    OSRestoreInterrupts(enabled);
}

static void startWorkingClose_(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    RFLiSetWorking(TRUE);
    RFLiGetManager()->beforeCloseErr = RFLiGetManager()->lastErrCode;
    RFLiGetManager()->beforeCloseReason = RFLiGetManager()->lastReason;
    RFLiGetManager()->lastErrCode = RFLErrcode_Busy;

    OSRestoreInterrupts(enabled);
}

static void endWorkingCloseReason_(RFLErrcode err, s32 reason) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    RFLiSetWorking(FALSE);
    if (err == RFLErrcode_Success) {
        RFLiGetManager()->lastErrCode = RFLiGetManager()->beforeCloseErr;
        // @bug Should be beforeCloseReason
        RFLiGetManager()->lastReason = RFLiGetManager()->beforeCloseErr;
    } else {
        RFLiGetManager()->lastErrCode = err;
        RFLiGetManager()->lastReason = reason;
    }

    OSRestoreInterrupts(enabled);
}

static void endWorkingClose_(RFLErrcode err) {
    endWorkingCloseReason_(err, NAND_RESULT_OK);
}

void RFLiEndWorkingReason(RFLErrcode err, s32 reason) {
    BOOL enabled;

    switch (RFLiGetManager()->lastErrCode) {
    case RFLErrcode_Busy:
    case RFLErrcode_Success:
        enabled = OSDisableInterrupts();

        RFLiSetWorking(FALSE);
        RFLiGetManager()->lastErrCode = err;
        RFLiGetManager()->lastReason = reason;

        OSRestoreInterrupts(enabled);
        break;
    default:
        break;
    }
}

void RFLiEndWorking(RFLErrcode err) {
    RFLiEndWorkingReason(err, NAND_RESULT_OK);
}

NANDCommandBlock* RFLiSetCommandBlock(RFLiFileType type, RFLiAsyncTag tag) {
    NANDCommandBlock* block;
    RFLiAccessInfo* info;
    RFLiCallbackTag* data;

    block = &RFLiGetAccInfo(type)->block;
    info = RFLiGetAccInfo(type);

    data = &info->tag;
    data->tag = tag;
    data->type = type;

    NANDSetUserData(block, data);
    return block;
}

RFLiFileType RFLiGetType(NANDCommandBlock* block) {
    const RFLiCallbackTag* data;
    data = (const RFLiCallbackTag*)NANDGetUserData(block);

    return data->type;
}

NANDFileInfo* RFLiGetWorkingFile(RFLiFileType type) {
    return RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;
}

static void alarmCallback_(OSAlarm* alarm, OSContext* ctx) {
#pragma unused(ctx)

    const RFLiFileType* userData;
    userData = (const RFLiFileType*)OSGetAlarmUserData(alarm);

    if (RFLAvailable()) {
        const RFLiExCallback alarmCallback =
            RFLiGetAccInfo(*userData)->retryCallback;
        alarmCallback(*userData);
    }
}

static void retry_(u32 arg, u8 retryCount, RFLiExCallback callback) {
    RFLiAccessInfo* info;

    if (retryCount < MAX_RETRY_COUNT) {
        info = RFLiGetAccInfo(arg);
        info->retryCallback = callback;
        info->alarmData = arg;
        info->retryCount = retryCount;

        OSCancelAlarm(&info->alarm);
        OSSetAlarmUserData(&info->alarm, &info->alarmData);
        OSSetAlarm(&info->alarm, OS_MSEC_TO_TICKS(50), alarmCallback_);
    } else {
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, NAND_RESULT_BUSY);
    }
}

static void opencallback_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;
    BOOL doCallback;

    doCallback = TRUE;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
        info->opened = TRUE;
        RFLiEndWorking(RFLErrcode_Success);
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_MAXFD:
    case NAND_RESULT_MAXBLOCKS:
    case NAND_RESULT_AUTHENTICATION:
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ACCESS:
        info->opened = FALSE;
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        break;
    case NAND_RESULT_NOEXISTS:
        info->opened = FALSE;
        if (info->openInfo.openMode == 1) {
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        } else {
            result = NANDPrivateCreateAsync(
                info->openInfo.path, info->openInfo.perm, info->openInfo.attr,
                createcallback_,
                RFLiSetCommandBlock(type, RFLiAsyncTag_CreateAsync));

            switch (result) {
            case NAND_RESULT_OK:
                doCallback = FALSE;
                break;
            default:
                RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
            }
        }
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_INVALID:
    default:
        info->opened = FALSE;
        RFLiEndWorkingReason(RFLErrcode_Fatal, result);
        break;
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

static void createcallback_(s32 result, NANDCommandBlock* block) {
    NANDCommandBlock* openBlock;
    RFLiAccessInfo* info;
    RFLiFileType type;
    BOOL doCallback;
    s32 reason;

    doCallback = TRUE;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
    case NAND_RESULT_EXISTS:
        openBlock = RFLiSetCommandBlock(type, RFLiAsyncTag_OpenAsync);
        memset(info->safeBuffer, 0, ACC_SAFE_BUFFER_SIZE);

        reason = NANDPrivateSafeOpenAsync(
            info->openInfo.path, RFLiGetWorkingFile(type),
            info->openInfo.openMode, info->safeBuffer, ACC_SAFE_BUFFER_SIZE,
            opencallback_, openBlock);

        switch (reason) {
        case NAND_RESULT_OK:
            doCallback = FALSE;
            break;
        case NAND_RESULT_NOEXISTS:
        case NAND_RESULT_MAXFD:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
            break;
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFLErrcode_Fatal, result);
            break;
        }
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ECC_CRIT:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, result);
        break;
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

static void close2opencallback_(s32 result, NANDCommandBlock* block) {
    NANDCommandBlock* openBlock;
    RFLiAccessInfo* info;
    RFLiFileType type;
    BOOL doCallback;
    s32 reason;

    doCallback = TRUE;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
        openBlock = RFLiSetCommandBlock(type, RFLiAsyncTag_OpenAsync);
        memset(info->safeBuffer, 0, ACC_SAFE_BUFFER_SIZE);

        reason = NANDPrivateSafeOpenAsync(
            info->openInfo.path, RFLiGetWorkingFile(type),
            info->openInfo.openMode, info->safeBuffer, ACC_SAFE_BUFFER_SIZE,
            opencallback_, openBlock);

        switch (reason) {
        case NAND_RESULT_OK:
            doCallback = FALSE;
            break;
        case NAND_RESULT_NOEXISTS:
        case NAND_RESULT_MAXFD:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
            break;
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFLErrcode_Fatal, result);
            break;
        }
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, result);
        break;
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

RFLErrcode RFLiOpenAsync(RFLiFileType type, u8 openMode,
                         RFLiCallback callback) {
    NANDCommandBlock* block;
    const char* filename;
    NANDFileInfo* file;
    RFLiAccessInfo* info;
    s32 reason;

    file = RFLiGetWorkingFile(type);
    RFLiStartWorking();
    info = RFLiGetAccInfo(type);
    filename = scFileNames[type];

    info->callback = callback;
    strncpy(info->openInfo.path, filename, 64);
    info->openInfo.path[64] = '\0';
    info->openInfo.openMode = openMode;
    info->openInfo.perm = scFilePermissions[type];
    info->openInfo.attr = scFileAttributes[type];

    block = RFLiSetCommandBlock(type, RFLiAsyncTag_OpenAsync);
    if (info->opened) {
        reason = NANDSafeCloseAsync(file, close2opencallback_, block);

        switch (reason) {
        case NAND_RESULT_OK:
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        case NAND_RESULT_CORRUPT:
        case NAND_RESULT_ACCESS:
        default:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
            break;
        }
    } else {
        memset(info->safeBuffer, 0, ACC_SAFE_BUFFER_SIZE);
        reason = NANDPrivateSafeOpenAsync(
            filename, file, openMode, info->safeBuffer, ACC_SAFE_BUFFER_SIZE,
            opencallback_, block);

        switch (reason) {
        case NAND_RESULT_OK:
            break;
        case NAND_RESULT_NOEXISTS:
            if (openMode == 1) {
                RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
            } else {
                block = RFLiSetCommandBlock(type, RFLiAsyncTag_CreateAsync);
                reason = NANDPrivateCreateAsync(filename, info->openInfo.perm,
                                                info->openInfo.attr,
                                                createcallback_, block);
                if (reason != NAND_RESULT_OK) {
                    RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
                }
            }
            break;
        case NAND_RESULT_MAXFD:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
            break;
        }
    }

    return RFLGetAsyncStatus();
}

static void readcallback_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;
    BOOL error;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    error = FALSE;

    // Non-error result = buffer size
    if (result >= NAND_RESULT_OK) {
        if (result > info->readInfo.size) {
            error = TRUE;
            RFLiEndWorking(RFLErrcode_Loadfail);
        } else if (result == info->readInfo.size) {
            RFLiEndWorking(RFLErrcode_Success);
        } else {
            error = TRUE;
            RFLiEndWorking(RFLErrcode_Loadfail);
        }
    } else {
        error = TRUE;

        switch (result) {
        case NAND_RESULT_ACCESS:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_CORRUPT:
        case NAND_RESULT_ECC_CRIT:
        case NAND_RESULT_AUTHENTICATION:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFLErrcode_Fatal, result);
            break;
        }
    }

    if (error) {
        memset(info->readInfo.dst, 0, info->readInfo.size);
    }

    if (info->callback != NULL) {
        info->callback();
    }
}

static void readseekcallback_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;
    BOOL doCallback;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    doCallback = TRUE;

    // Non-error result = Seek offset
    if (result == info->readInfo.offset) {
        NANDCommandBlock* block;
        NANDFileInfo* file;
        s32 reason;

        block = RFLiSetCommandBlock(type, RFLiAsyncTag_ReadAsync);
        file = RFLiGetWorkingFile(type);
        reason = NANDReadAsync(file, info->readInfo.dst,
                               ROUND_UP(info->readInfo.size, 32), readcallback_,
                               block);
        switch (reason) {
        case NAND_RESULT_OK:
            doCallback = FALSE;
            break;
        case NAND_RESULT_ACCESS:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_CORRUPT:
        case NAND_RESULT_ECC_CRIT:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
            break;
        }
    } else if (result >= NAND_RESULT_OK) {
        RFLiEndWorking(RFLErrcode_Loadfail);
    } else {
        switch (result) {
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
            break;
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFLErrcode_Fatal, result);
            break;
        }
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

RFLErrcode RFLiReadAsync(RFLiFileType type, void* dst, u32 size,
                         RFLiCallback callback, s32 offset) {
    NANDCommandBlock* block;
    RFLiAccessInfo* info;
    NANDFileInfo* file;
    s32 reason;

    file = RFLiGetWorkingFile(type);
    info = RFLiGetAccInfo(type);

    RFLiStartWorking();

    info->callback = callback;
    info->readInfo.dst = dst;
    info->readInfo.size = size;
    info->readInfo.offset = offset;

    block = RFLiSetCommandBlock(type, RFLiAsyncTag_SeekAsync);
    reason =
        NANDSeekAsync(file, offset, 0, readseekcallback_, block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static void retryWrite_(u32 arg) {
    NANDCommandBlock* block;
    RFLiAccessInfo* info;
    NANDFileInfo* file;
    s32 reason;

    info = RFLiGetAccInfo(arg);
    block = RFLiSetCommandBlock(arg, RFLiAsyncTag_WriteAsync);
    file = RFLiGetWorkingFile(arg);

    reason = NANDWriteAsync(file, info->writeInfo.src, info->writeInfo.size,
                            writecallback_, block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        retry_(arg, info->retryCount + 1, retryWrite_);
        break;
    case NAND_RESULT_MAXBLOCKS:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }

    if (!RFLiIsWorking() && info->callback != NULL) {
        info->callback();
    }
}

static void writecallback_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    if (result == info->writeInfo.size) {
        RFLiEndWorking(RFLErrcode_Success);
    } else if (result >= NAND_RESULT_OK) {
        RFLiEndWorking(RFLErrcode_Savefail);
    } else {
        switch (result) {
        case NAND_RESULT_MAXBLOCKS:
        case NAND_RESULT_CORRUPT:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
            break;
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
            retry_(type, info->retryCount + 1, retryWrite_);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFLErrcode_Fatal, result);
            break;
        }
    }

    if (info->callback != NULL) {
        info->callback();
    }
}

static void retryWriteSeek_(u32 arg) {
    NANDCommandBlock* block;
    RFLiAccessInfo* info;
    NANDFileInfo* file;
    s32 reason;

    info = RFLiGetAccInfo(arg);
    block = RFLiSetCommandBlock(arg, RFLiAsyncTag_SeekAsync);
    file = RFLiGetWorkingFile(arg);

    reason = NANDSeekAsync(file, info->writeInfo.offset, 0,
                           writeseekcallback_, block);
    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        retry_(arg, info->retryCount + 1, retryWriteSeek_);
        break;
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }
}

static void writeseekcallback_inline(u32 arg) {
    NANDCommandBlock* block;
    RFLiAccessInfo* info;
    NANDFileInfo* file;
    s32 reason;

    block = RFLiSetCommandBlock(arg, RFLiAsyncTag_WriteAsync);
    info = RFLiGetAccInfo(arg);
    file = RFLiGetWorkingFile(arg);
    reason = NANDWriteAsync(file, info->writeInfo.src, info->writeInfo.size,
                            writecallback_, block);
    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        retry_(arg, 0, retryWrite_);
        break;
    case NAND_RESULT_MAXBLOCKS:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }
}

static void writeseekcallback_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;
    BOOL doCallback;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    doCallback = TRUE;

    if (result == info->writeInfo.offset) {
        writeseekcallback_inline(type);
        if (RFLiIsWorking()) {
            doCallback = FALSE;
        }
    } else if (result >= NAND_RESULT_OK) {
        RFLiEndWorking(RFLErrcode_Savefail);
    } else {
        switch (result) {
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
            retry_(type, 0, retryWriteSeek_);
            break;
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFLErrcode_Fatal, result);
            break;
        }
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

RFLErrcode RFLiWriteAsync(RFLiFileType type, const void* src, u32 size,
                          RFLiCallback callback, s32 offset) {
    NANDCommandBlock* block;
    RFLiAccessInfo* info;
    NANDFileInfo* file;
    s32 reason;

    info = RFLiGetAccInfo(type);
    file = RFLiGetWorkingFile(type);

    RFLiStartWorking();

    info->callback = callback;
    info->writeInfo.src = src;
    info->writeInfo.size = size;
    info->writeInfo.offset = offset;

    block = RFLiSetCommandBlock(type, RFLiAsyncTag_SeekAsync);
    reason =
        NANDSeekAsync(file, offset, 0, writeseekcallback_, block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        retry_(type, 0, retryWriteSeek_);
        break;
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static s32 closecore_(u32 arg) {
    NANDCommandBlock* block;
    NANDFileInfo* info;

    block = RFLiSetCommandBlock(arg, RFLiAsyncTag_CloseAsync);
    info = RFLiGetWorkingFile(arg);

    return NANDSafeCloseAsync(info, closecallback_, block);
}

static void closecallback_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
        info->opened = FALSE;
        endWorkingClose_(RFLErrcode_Success);
        break;
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, result);
        break;
    }

    if (!RFLiIsWorking() && info->callback != NULL) {
        info->callback();
    }
}

RFLErrcode RFLiCloseAsync(RFLiFileType type, RFLiCallback callback) {
    RFLiAccessInfo* info;
    s32 reason;

    info = RFLiGetAccInfo(type);
    startWorkingClose_();
    info->callback = callback;
    reason = closecore_(type);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static void getlengthcallback_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
        RFLiEndWorking(RFLErrcode_Success);
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, result);
        break;
    }

    if (info->callback != NULL) {
        info->callback();
    }
}

RFLErrcode RFLiGetLengthAsync(RFLiFileType type, u32* out,
                              RFLiCallback callback) {
    NANDCommandBlock* block;
    NANDFileInfo* file;
    RFLiAccessInfo* info;
    s32 reason;

    info = RFLiGetAccInfo(type);
    RFLiStartWorking();
    info->callback = callback;
    info->getLengthInfo.dst = out;

    block = RFLiSetCommandBlock(type, RFLiAsyncTag_GetLengthAsync);
    file = RFLiGetWorkingFile(type);
    reason = NANDGetLengthAsync(file, out, getlengthcallback_, block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static void deletecallback_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
    case NAND_RESULT_NOEXISTS:
        RFLiEndWorking(RFLErrcode_Success);
        break;
    case NAND_RESULT_OPENFD:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, result);
        break;
    }

    if (info->callback != NULL) {
        info->callback();
    }
}

RFLErrcode RFLiDeleteAsync(RFLiFileType type, RFLiCallback callback) {
    NANDCommandBlock* block;
    RFLiAccessInfo* info;
    s32 reason;

    info = RFLiGetAccInfo(type);
    RFLiStartWorking();
    info->callback = callback;

    block = RFLiSetCommandBlock(type, RFLiAsyncTag_DeleteAsync);
    reason = NANDPrivateDeleteAsync(scFileNames[type], deletecallback_, block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_NOEXISTS:
        RFLiEndWorking(RFLErrcode_Success);
        break;
    case NAND_RESULT_OPENFD:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static void createDirCommon_(const char* dir, NANDAsyncCallback callback) {
    NANDCommandBlock* block;
    s32 reason;

    block =
        RFLiSetCommandBlock(RFLiFileType_Database, RFLiAsyncTag_CreateDirAsync);
    reason =
        NANDPrivateCreateDirAsync(dir, NAND_PERM_RWALL, 0, callback, block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_EXISTS:
        callback(NAND_RESULT_EXISTS, block);
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, reason);
        break;
    }
}

static void createdircallback2_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
    case NAND_RESULT_EXISTS:
        RFLiEndWorking(RFLErrcode_Success);
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, result);
        break;
    }

    if (RFLGetAsyncStatus() != RFLErrcode_Busy && info->callback != NULL) {
        info->callback();
    }
}

static void createdircallback1_(s32 result, NANDCommandBlock* block) {
    RFLiAccessInfo* info;
    RFLiFileType type;

    type = RFLiGetType(block);
    info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
    case NAND_RESULT_EXISTS:
        createDirCommon_(scSecondDirectory, createdircallback2_);
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFLErrcode_NANDCommandfail, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFLErrcode_Fatal, result);
        break;
    }

    if (RFLGetAsyncStatus() != RFLErrcode_Busy && info->callback != NULL) {
        info->callback();
    }
}

RFLErrcode RFLiCreateSaveDirAsync(RFLiCallback callback) {
    RFLiAccessInfo* info;

    info = RFLiGetAccInfo(RFLiFileType_Database);
    RFLiStartWorking();
    info->callback = callback;

    createDirCommon_(scFirstDirectory, createdircallback1_);

    return RFLGetAsyncStatus();
}
