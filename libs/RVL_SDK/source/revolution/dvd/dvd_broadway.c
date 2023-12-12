#include <revolution/os.h>
#include <revolution/esp.h>
#include <revolution/ipc.h>
#include <stdbool.h>
#include <cstring>

IOSFd DiFD = -1;

static volatile bool requestInProgress = false;
static bool breakRequested;
static bool callbackInProgress;

static u32 registerBuf[8] __attribute__((aligned(32)));
static u32 statusRegister[8] __attribute__((aligned(32)));
static s32 lastTicketError[8] __attribute__((aligned(32)));

static u32 readLength;
static u32 spinUpValue;

static diRegVals_t diRegValCache __attribute__((aligned(32)));

static bool DVDLowInitCalled = false;

typedef enum callbackType {
    BOGUS_TYPE = 0,
    TRANSACTION_CB,
    COVER_CB,
    COVER_REG_CB
} callbackType_t;

static int freeCommandBuf = 0;
static diCommand_t* diCommand;
static char* pathBuf;

typedef struct dvdContext {
    DVDLowCallback callback;
    callbackType_t callbackType;
    bool inUse; 
    u32 contextMagic;
    u32 contextNum;
    u32 pad[3];
} dvdContext_t;

static int freeDvdContext = 0;
static bool dvdContextsInited = false;
static dvdContext_t dvdContexts[4] __attribute__((aligned(32)));
static IOSIoVector ioVec[10] __attribute__((aligned(32)));

IOSError doTransactionCallback(IOSError ret, void* context) {
    dvdContext_t* dvdContext = context;

    if (dvdContext->contextMagic != 0xFEEBDAED) {
        OSReport("(doTransactionCallback) Error - context mangled!\n");
        dvdContext->contextMagic = 0xFEEBDAED;
        goto out;
    }

    requestInProgress = false;

    if (dvdContext->callback != NULL) {
        int callbackArg;
        callbackInProgress = TRUE;
        callbackArg = ret;

        if (breakRequested == TRUE) {
            breakRequested = false;
            callbackArg |= 8;
        }

        if (callbackArg & 1) {
            readLength = 0;
        }

        dvdContext->callback((u32)callbackArg);
        callbackInProgress = false;
    }

out:
    dvdContext->inUse = false;
    return 0;
}

IOSError doPrepareCoverRegisterCallback(IOSError ret, void* context) {
    dvdContext_t* dvdContext;
    requestInProgress = false;

    diRegValCache.CoverRegVal = registerBuf[0];
    dvdContext = (dvdContext_t*)context;

    if (dvdContext->contextMagic != 0xFEEBDAED) {
        OSReport("(doTransactionCallback) Error - context mangled!\n");
        dvdContext->contextMagic = 0xFEEBDAED;
    }
    else {
        if (dvdContext->callback != 0) {
            callbackInProgress = true;

            if (breakRequested == true) {
                breakRequested = false;
                ret |= 8;
            }

            dvdContext->callback(ret);
            callbackInProgress = false;
        }
    }

    dvdContext->inUse = false;
    return 0;
}

static void* ddrAllocAligned32(const int size) {
    void* low, *high;

    if ((size & 0x1F) != 0) {
        return 0;
    }

    low = IPCGetBufferLo();
    high = IPCGetBufferHi();

    if (((u32)low & 0x1F) != 0) {
        low = (void*)(((u32)low + 31) & 0x1F);
    }

    if ((u32)low + size > (u32)high) {
        OSReport("(ddrAllocAligned32) Not enough space to allocate %d bytes\n", size);
    }

    IPCSetBufferLo((void*)((u32)low + size));
    return low;
}

static bool allocateStructures(void) {
    if ((diCommand = ddrAllocAligned32(sizeof(diCommand_t) * 4)) == 0) {
        OSReport("Allocation of diCommand blocks failed\n");
        return false;
    }

    if ((pathBuf = ddrAllocAligned32(32)) == 0) {
        OSReport("Allocation of pathBuf failed\n");
        return false;
    }

    return true;    
}

static void initDvdContexts(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        dvdContexts[i].callback = 0;
        dvdContexts[i].callbackType = 0;
        dvdContexts[i].inUse = false;
        dvdContexts[i].contextMagic = 0xFEEBDAED;
        dvdContexts[i].contextNum = i;
    }

    freeDvdContext = 0;
}

static inline dvdContext_t* newContext(const DVDLowCallback callback, const callbackType_t type) {
    int returnIndex;
    bool use = dvdContexts[freeDvdContext].inUse != 0;

    if (use == 1)  {
        OSReport("(newContext) ERROR: freeDvdContext.inUse (#%d) is true\n", freeDvdContext);
        OSReport("(newContext) Now spinning in infinite loop\n");

        while (1) {

        }
    }

    if (dvdContexts[freeDvdContext].contextMagic != 0xFEEBDAED) {
        OSReport("(newContext) Something overwrote the context magic - spinning \n");
    
        while (1) {

        }
    }

    dvdContexts[freeDvdContext].callback = callback;
    dvdContexts[freeDvdContext].callbackType = type;
    dvdContexts[freeDvdContext].inUse = true;
    returnIndex = freeDvdContext;
    freeDvdContext++;

    if (freeDvdContext >= 4) {
        freeDvdContext = 0;
    }

    return(dvdContexts + returnIndex);
}

static inline void nextCommandBuf(int* bufNum) {
    (*bufNum)++;

    if (*bufNum >= 4) {
        *bufNum = 0;
    }
}

bool DVDLowFinalize(void) {
    IOSError ret = IOS_Close(DiFD);

    if (ret != IOS_ERROR_OK) {
        OSReport("(DVDLowFinish) Error: IOS_Close failed\n");
        return false;
    }

    DVDLowInitCalled = false;
    return true;
}

bool DVDLowInit(void) {
    IOSError retVal;

    if (DVDLowInitCalled == false) {
        DVDLowInitCalled = true;
        retVal = IPCCltInit();

        if (retVal != IOS_ERROR_OK) {
            OSReport("IPCCltInit returned error: %d\n", retVal);
            return false;
        }

        if (allocateStructures() == false) {
            return false;
        }

        if (dvdContextsInited == false) {
            initDvdContexts();
            dvdContextsInited = true;
        }
    }

    strncpy(pathBuf, "/dev/di", 32);
    DiFD = IOS_Open(pathBuf, 0);

    if (DiFD >= 0) {
        return true;
    }
    else {
        switch (DiFD) {
            case IOS_ERROR_NOEXISTS:
                OSReport("(DVDLowInit) Error: IOS_Open failed - pathname '/dev/di' does not exist\n");
                return false;
                break;
            case IOS_ERROR_ACCESS:
                OSReport("(DVDLowInit) Error: IOS_Open failed - calling thread lacks permission\n");
                return false;
                break;
            case IOS_ERROR_MAX:
                OSReport("(DVDLowInit) Error: IOS_Open failed - connection limit has been reached\n");
                return false;
                break;
            default:
                OSReport("(DVDLowInit) IOS_Open failed, errorcode = %d\n", DiFD);
                return false;
                break;
        }
    }
}

bool DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;
    requestInProgress = true;
    dvdContext = newContext(callback, 1);

    if (diskID == 0) {
        OSReport("@@@@@@ WARNING - Calling DVDLowReadDiskId with NULL ptr\n");
    }

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x70;

    rv = IOS_IoctlAsync(DiFD, 0x70, &diCommand[freeCommandBuf], sizeof(diCommand_t), diskID, sizeof(DVDDiskID), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowReadDiskID) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowOpenPartition(const u32 partitionWordOffset, const ESTicket* const eTicket, const u32 numCertBytes, const u8* const certificates, ESTitleMeta* tmd, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;
    
    if (eTicket != 0 && !((u32)eTicket & 0x1F)) {
        OSReport("(DVDLowOpenPartition) eTicket memory is unaligned\n");
        return false;
    }

    if (certificates != 0 && !((u32)certificates & 0x1F)) {
        OSReport("(DVDLowOpenPartition) certificates memory is unaligned\n");
        return false;
    }

    if (tmd != 0 && !((u32)tmd & 0x1F)) {
        OSReport("(DVDLowOpenPartition) certificates memory is unaligned\n");
        return false;
    }

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x8B;
    diCommand[freeCommandBuf].arg[0] = partitionWordOffset;
    ioVec[0].base = (u8*)&diCommand[freeCommandBuf];
    ioVec[0].length = sizeof(diCommand_t);

    ioVec[1].base = (u8*)eTicket;
    if (eTicket == 0) {
        ioVec[1].length = 0;
    }
    else {
        ioVec[1].length = sizeof(ESTicket);
    }

    ioVec[2].base = (u8*)certificates;
    if (certificates == 0) {
        ioVec[2].length = 0;
    }
    else {
        ioVec[2].length = numCertBytes; 
    }

    ioVec[3].base = (u8*)tmd;
    ioVec[3].length = sizeof(ESTitleMeta);

    ioVec[4].base = (u8*)&lastTicketError[0];
    ioVec[4].length = sizeof(lastTicketError);

    rv = IOS_IoctlvAsync(DiFD, 0x8B, 3, 2, ioVec, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowOpenPartition) IOS_IoctlvAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowOpenPartitionWithTmdAndTicketView(const u32 partitionWordOffset, const ESTicketView* const eTicketView, const u32 numTmdBytes, const ESTitleMeta* const tmd, const u32 numCertBytes, const u8* const certificates, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;
    
    if (certificates != 0 && !((u32)certificates & 0x1F)) {
        return false;
    }

    if (tmd == 0) {
        OSReport("(%s) tmd parameter cannot be NULL\n", __FUNCTION__);
        return false;
    }
    else if (!((u32)tmd & 0x1F)) {
        OSReport("(%s) tmd memory is unaligned\n", __FUNCTION__);
        return false;
    }

    if (eTicketView == 0) {
        OSReport("(%s) eTicketView parameter cannot be NULL\n", __FUNCTION__);
        return false;
    }
    else if (!((u32)eTicketView & 0x1F)) {
        OSReport("(%s) eTicketView memory is unaligned\n", __FUNCTION__);
        return false;
    }

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x94;
    diCommand[freeCommandBuf].arg[0] = partitionWordOffset;
    ioVec[0].base = (u8*)&diCommand[freeCommandBuf];
    ioVec[0].length = sizeof(diCommand_t);

    ioVec[1].base = (u8*)eTicketView;
    ioVec[1].length = sizeof(ESTicketView);

    ioVec[2].base = (u8*)tmd;
    ioVec[2].length = numTmdBytes;

    ioVec[3].base = (u8*)certificates;
    if (certificates == 0) {
        ioVec[3].length = 0;
    }
    else {
        ioVec[3].length = numCertBytes; 
    }

    ioVec[4].base = (u8*)&lastTicketError[0];
    ioVec[4].length = sizeof(lastTicketError);

    rv = IOS_IoctlvAsync(DiFD, 0x94, 4, 1, ioVec, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowOpenPartition) IOS_IoctlvAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowGetNoDiscBufferSizes(const u32 partitionWordOffset, u32* numTmdBytes, u32* numCertBytes, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    if (numTmdBytes == 0 || numCertBytes == 0) {
        OSReport("(%s) Error: NULL pointer argument\n", __FUNCTION__);
        return false;
    }

    if (!((u32)numTmdBytes & 0x1F)) {
        OSReport("(%s) numTmdBytes memory is unaligned\n", __FUNCTION__);
        return false;
    }

    if (!((u32)numCertBytes & 0x1F)) {
        OSReport("(%s) certificates memory is unaligned\n", __FUNCTION__);
        return false;
    }

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x92;
    diCommand[freeCommandBuf].arg[0] = partitionWordOffset;

    ioVec[0].base = (u8*)&diCommand[freeCommandBuf];
    ioVec[0].length = sizeof(diCommand_t);

    ioVec[1].base = (u8*)numTmdBytes;
    ioVec[1].length = 4;

    ioVec[2].base = (u8*)numCertBytes;
    ioVec[2].length = 4;

    rv = IOS_IoctlvAsync(DiFD, 0x92, 1, 2, ioVec, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (%s) IOS_IoctlvAsync returned error: %d\n", __FUNCTION__, rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

#define is_aligned(addr) (((u32)(addr) & 0x1F) == 0)

bool DVDLowGetNoDiscOpenPartitionParams(const u32 partitionWordOffset, ESTicket* eTicket, u32* numTmdBytes, ESTitleMeta* tmd, u32* numCertBytes, u8* certificates, u32* dataWordOffset, u8* h3HashPtr, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    if (eTicket == 0 || numTmdBytes == 0 || tmd == 0 || numCertBytes == 0 || certificates == 0 || dataWordOffset == 0 || h3HashPtr == 0) {
        OSReport("(%s) Error: NULL pointer argument\n", __FUNCTION__);
        return false;
    }

    if (!is_aligned(eTicket) || !is_aligned(numTmdBytes) || !is_aligned(tmd) || !is_aligned(numCertBytes) || !is_aligned(certificates) || !is_aligned(dataWordOffset) || !is_aligned(h3HashPtr)) {
        OSReport("(%s) pointer argument is unaligned\n", __FUNCTION__);
        return false;
    }

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x90;
    diCommand[freeCommandBuf].arg[0] = partitionWordOffset;

    ioVec[0].base = (u8*)&diCommand[freeCommandBuf];
    ioVec[0].length = sizeof(diCommand_t);

    ioVec[1].base = (u8*)numTmdBytes;
    ioVec[1].length = 4;

    ioVec[2].base = (u8*)numCertBytes;
    ioVec[2].length = 4;

    ioVec[3].base = (u8*)eTicket;
    ioVec[3].length = sizeof(ESTicket);

    ioVec[4].base = (u8 *) numTmdBytes;
    ioVec[4].length = 4;

    ioVec[5].base = (u8*)tmd;
    ioVec[5].length = *numTmdBytes;

    ioVec[6].base = (u8*)numCertBytes;
    ioVec[6].length = 4;

    ioVec[7].base = certificates;
    ioVec[7].length = *numCertBytes;

    ioVec[8].base = (u8*)dataWordOffset;
    ioVec[8].length = 4;

    ioVec[9].base = h3HashPtr;
    ioVec[9].length = 98304;

    rv = IOS_IoctlvAsync(DiFD, 0x90, 3, 7, ioVec, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (%s) IOS_IoctlvAsync returned error: %d\n", __FUNCTION__, rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowClosePartition(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x8C;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x8C, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowClosePartition) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowUnencryptedRead(void* destAddr, u32 length, u32 wordOffset, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    readLength = length;
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x8D;
    diCommand[freeCommandBuf].arg[0] = length;
    diCommand[freeCommandBuf].arg[1] = wordOffset;

    rv = IOS_IoctlAsync(DiFD, 0x8D, &diCommand[freeCommandBuf], sizeof(diCommand_t), destAddr, length, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowUnencryptedRead) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowStopMotor(bool eject, bool saving, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xE3;
    diCommand[freeCommandBuf].arg[0] = eject;
    diCommand[freeCommandBuf].arg[1] = saving;

    rv = IOS_IoctlAsync(DiFD, 0xE3, &diCommand[freeCommandBuf], sizeof(diCommand_t), &diRegValCache, sizeof(diRegVals_t), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowStopMotor) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowInquiry(DVDDriveInfo* info, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x12;
    rv = IOS_IoctlAsync(DiFD, 0x12, &diCommand[freeCommandBuf], sizeof(diCommand_t), info, sizeof(DVDDriveInfo), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowInquiry) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowRequestError(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xE0;
    rv = IOS_IoctlAsync(DiFD, 0xE0, &diCommand[freeCommandBuf], sizeof(diCommand_t), &diRegValCache, sizeof(diRegVals_t), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowInquiry) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowSetSpinupFlag(u32 spinUp) {
    spinUpValue = spinUp;
    return true;
}

bool DVDLowReset(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x8A;
    diCommand[freeCommandBuf].arg[0] = spinUpValue;
    rv = IOS_IoctlAsync(DiFD, 0x8A, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowInquiry) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowAudioBufferConfig(bool enable, u32 size, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xE4;
    diCommand[freeCommandBuf].arg[0] = enable;
    diCommand[freeCommandBuf].arg[1] = size;
    rv = IOS_IoctlAsync(DiFD, 0xE4, &diCommand[freeCommandBuf], sizeof(diCommand_t), &diRegValCache, sizeof(diRegVals_t), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowAudioBufferConfig) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowReportKey(DVDVideoReportKey* reportKey, u32 format, u32 lsn, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xA4;
    diCommand[freeCommandBuf].arg[0] = format >> 16;
    diCommand[freeCommandBuf].arg[1] = lsn;

    rv = IOS_IoctlAsync(DiFD, 0xA4, &diCommand[freeCommandBuf], sizeof(diCommand_t), reportKey, sizeof(DVDVideoReportKey), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowReportKey) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowSetMaximumRotation(u32 subcmd, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xDD;
    diCommand[freeCommandBuf].arg[0] = (subcmd >> 16) & 3;
    rv = IOS_IoctlAsync(DiFD, 0xDD, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowSetMaximumRotation) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowRead(void* destAddr, u32 length, u32 wordOffset, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    if (((u32)destAddr & 0x1F) != 0) {
        OSReport("(DVDLowRead): ERROR - destAddr buffer is not 32 byte aligned\n");
        return false;
    }

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    readLength = length;
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x71;
    diCommand[freeCommandBuf].arg[0] = length;
    diCommand[freeCommandBuf].arg[1] = wordOffset;
    rv = IOS_IoctlAsync(DiFD, 0x71, &diCommand[freeCommandBuf], sizeof(diCommand_t), destAddr, length, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowRead) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowSeek(u32 wordOffset, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xAB;
    diCommand[freeCommandBuf].arg[0] = wordOffset;
    rv = IOS_IoctlAsync(DiFD, 0xAB, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowSeek) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

u32 DVDLowGetCoverRegister(void) {
    return diRegValCache.CoverRegVal;
}

u32 DVDLowGetStatusRegister(void) {
    return statusRegister[0];
}

bool DVDLowPrepareCoverRegister(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x7A;
    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x7A, &diCommand[freeCommandBuf], sizeof(diCommand_t), registerBuf, sizeof(registerBuf), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowPrepareCoverRegister) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

bool DVDLowPrepareStatusRegister(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x95;
    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x95, &diCommand[freeCommandBuf], sizeof(diCommand_t), statusRegister, sizeof(statusRegister), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowPrepareStatusRegister) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

u32 DVDLowGetImmBufferReg(void) {
    return diRegValCache.ImmRegVal;
}

bool DVDLowUnmaskStatusInterrupts(void) {
    return true;
}

bool DVDLowMaskCoverInterrupt(void) {
    return true;
}

bool DVDLowClearCoverInterrupt(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x86;
    requestInProgress = true;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x86, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowClearCoverInterrupt) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = false;
        return false;
    }

    return true;
}

BOOL __DVDLowTestAlarm(const OSAlarm *) {
    return FALSE;
}