#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

static BOOL NWC24iIsRequestPending = FALSE;

static s32 CallbackAsyncIpc(s32 result, void* pArg);

NWC24Err NWC24iOpenResourceManager(const char* pUser, const char* pName, s32* pFd, u32 mode) {
    s32 result;

    if (pFd == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }

    result = IOS_Open(pName, mode);
    *pFd = result;

    if (result < 0) {
        if (result == IOS_ERROR_NOEXISTS) {
            return NWC24_ERR_INPROGRESS;
        }

        if (result == IOS_ERROR_QFULL) {
            return NWC24_ERR_BUSY;
        }

        return NWC24_ERR_INTERNAL_IPC;
    }

    return NWC24_OK;
}

NWC24Err NWC24iCloseResourceManager(const char* pUser, s32 fd) {
    return IOS_Close(fd) < 0 ? NWC24_ERR_INTERNAL_IPC : NWC24_OK;
}

NWC24Err NWC24iIoctlResourceManager(const char* pUser, s32 fd, s32 type, void* pIn, s32 inSize, void* pOut, s32 outSize) {
    return IOS_Ioctl(fd, type, pIn, inSize, pOut, outSize) < 0 ? NWC24_ERR_INTERNAL_IPC : NWC24_OK;
}

NWC24Err NWC24iIoctlResourceManagerAsync(const char* pUser, s32 fd, s32 type, void* pIn, s32 inSize, void* pOut, s32 outSize, void* pCallbackArg) {
    if (IOS_IoctlAsync(fd, type, pIn, inSize, pOut, outSize, CallbackAsyncIpc, pCallbackArg) < 0) {
        return NWC24_ERR_INTERNAL_IPC;
    }

    NWC24iIsRequestPending = TRUE;
    return NWC24_OK;
}

BOOL NWC24iIsAsyncRequestPending(void) {
    return NWC24iIsRequestPending;
}

static s32 CallbackAsyncIpc(s32 result, void* pArg) {
    if (pArg != NULL) {
        *(s32*)pArg = result;
    }

    NWC24iIsRequestPending = FALSE;
    return IOS_ERROR_OK;
}
