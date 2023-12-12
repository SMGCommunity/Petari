#include <revolution/ipc.h>
#include <revolution/nwc24.h>

static BOOL NWC24iIsRequestPending = FALSE;

IOSError CallbackAsyncIpc(IOSError, void *);

NWC24Err NWC24iOpenResourceManager(const char* funcName, const char* path, IOSFd* pFd, u32 flags) {
    if (!pFd) {
        return NWC24_ERR_INVALID_VALUE;
    }

    *pFd = IOS_Open(path, flags);

    if (*pFd < IOS_ERROR_OK) {
        if (*pFd == IOS_ERROR_NOEXISTS) {
            return NWC24_ERR_INPROGRESS;
        }
        else if (*pFd == IOS_ERROR_QFULL) {
            return NWC24_ERR_BUSY;
        }
        else {
            return NWC24_ERR_INTERNAL_IPC;
        }
    }

    return NWC24_OK;
}

NWC24Err NWC24iCloseResourceManager(const char* funcName, IOSFd fd) {
    IOSError err = IOS_Close(fd);

    if (err < IOS_ERROR_OK) {
        return NWC24_ERR_INTERNAL_IPC;
    }

    return NWC24_OK;
}

NWC24Err NWC24iIoctlResourceManager(const char* funcName, IOSFd fd, s32 cmd, void* input, u32 input_bytes, void* output, u32 output_bytes) {
    IOSError err = IOS_Ioctl(fd, cmd, input, input_bytes, output, output_bytes);

    if (err < IOS_ERROR_OK) {
        return NWC24_ERR_INTERNAL_IPC;
    }

    return NWC24_OK;
}

NWC24Err NWC24iIoctlResourceManagerAsync(const char* funcName, IOSFd fd, s32 cmd, void* input, u32 input_bytes, void* output, u32 output_bytes, IOSError* ret) {
    IOSError err = IOS_IoctlAsync(fd, cmd, input, input_bytes, output, output_bytes, CallbackAsyncIpc, (void*)ret);

    if (err < IOS_ERROR_OK) {
        return NWC24_ERR_INTERNAL_IPC;
    }

    NWC24iIsRequestPending = TRUE;
    return NWC24_OK;
}

BOOL NWC24iIsAsyncRequestPending(void) {
    return NWC24iIsRequestPending;
}

IOSError CallbackAsyncIpc(IOSError ret, void* ctxt) {
    if (ctxt != NULL) {
        *(IOSError*)ctxt = ret;
    }

    NWC24iIsRequestPending = FALSE;
    return IOS_ERROR_OK;
}