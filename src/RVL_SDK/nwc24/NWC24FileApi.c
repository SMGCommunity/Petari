#include "revolution/nwc24/NWC24FileApi.h"
#include "revolution/nwc24.h"
#include "revolution/os/OSTime.h"

#define NAND_RETRY_COUNT 3

#define INVALIDATE_ALIGN (1 << 31)
#define INSTANCE_IDGEN_INIT (1 << 31)

#define SLEEP_MSEC(x) OSSleepTicks(OS_MSEC_TO_TICKS((s64)(x)))

static u32 RdBufferMutex = 0;
static u32 WrBufferMutex = 0;
static u32 InstanceIdGen = 0;

NWC24Err NWC24FOpen(NWC24File* pFile, const char* pPath, u32 mode) {
    InstanceIdGen++;
    InstanceIdGen |= INSTANCE_IDGEN_INIT;

    pFile->id = InstanceIdGen;
    pFile->align = 0;
    pFile->mode = mode;

    if (mode == NWC24_OPEN_NAND_WBUFF || mode == NWC24_OPEN_NAND_RBUFF || mode == NWC24_OPEN_VF_WBUFF || mode == NWC24_OPEN_VF_RBUFF) {
        if (!NWC24IsMsgLibOpened() && !NWC24IsMsgLibOpenedByTool()) {
            return NWC24_ERR_LIB_NOT_OPENED;
        }
    }

    if (mode == NWC24_OPEN_NAND_WBUFF || mode == NWC24_OPEN_VF_WBUFF) {
        if (WrBufferMutex != 0) {
            return NWC24_ERR_MUTEX;
        }

        WrBufferMutex = pFile->id;
    }

    if (mode & NWC24_OPEN_VF) {
        return NWC24iFOpenVF(pFile, pPath, mode);
    }

    return NWC24iFOpenNand(pFile, pPath, mode);
}

NWC24Err NWC24iFOpenNand(NWC24File* pFile, const char* pPath, u32 mode) {
    s32 result;
    NANDAccessType access;
    u32 i;

    switch (mode) {
    case NWC24_OPEN_NAND_W:
    case NWC24_OPEN_NAND_WBUFF: {
        result = NANDPrivateCreate(pPath, NAND_PERM_RWALL, 0);
        if (result != NAND_RESULT_OK && result != NAND_RESULT_EXISTS) {
            return NWC24_ERR_FILE_OTHER;
        }
        access = NAND_ACCESS_WRITE;
        break;
    }

    case NWC24_OPEN_NAND_R:
    case NWC24_OPEN_NAND_RBUFF: {
        access = NAND_ACCESS_READ;
        break;
    }

    case NWC24_OPEN_NAND_RW: {
        access = NAND_ACCESS_RW;
        break;
    }

    default: {
        return NWC24_ERR_INVALID_VALUE;
    }
    }

    for (i = 0; i < NAND_RETRY_COUNT; i++) {
        result = NANDPrivateOpen(pPath, &pFile->nandf, access);
        if (result != NAND_RESULT_BUSY) {
            break;
        }

        SLEEP_MSEC(1000);
    }

    if (result != NAND_RESULT_OK) {
        if (mode == NWC24_OPEN_NAND_WBUFF) {
            WrBufferMutex = 0;
        }

        if (result == NAND_RESULT_NOEXISTS) {
            return NWC24_ERR_FILE_NOEXISTS;
        }

        return NWC24_ERR_FILE_OPEN;
    }

    return NWC24_OK;
}

/*
NWC24Err NWC24iFOpenVF(NWC24File* pFile, const char* pPath, u32 mode) {
    s32 result;
    const char* access;

    switch (mode) {
    case NWC24_OPEN_VF_W:
    case NWC24_OPEN_VF_WBUFF: {
        access = "w";
        break;
    }

    case NWC24_OPEN_VF_R:
    case NWC24_OPEN_VF_RBUFF: {
        access = "r";
        break;
    }

    default: {
        return NWC24_ERR_INVALID_VALUE;
    }
    }

    pFile->vff = VFOpenFile(pPath, access, 0);

    if (pFile->vff == NULL) {
        result = VFGetLastError();

        if (mode == NWC24_OPEN_VF_WBUFF) {
            WrBufferMutex = 0;
        }

        if (*access == 'r' && result == VF_ERROR_0002) {
            return NWC24_ERR_FILE_NOEXISTS;
        }

        return ConvertVfError(result, NWC24_ERR_FILE_OPEN);
    }

    return NWC24_OK;
}
*/
