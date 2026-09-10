#ifndef RVL_SDK_NWC24_INTERNAL_IPC_H
#define RVL_SDK_NWC24_INTERNAL_IPC_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/ipc.h>
#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

NWC24Err NWC24iOpenResourceManager(const char* pUser, const char* pName, s32* pFd, u32 mode);

NWC24Err NWC24iCloseResourceManager(const char* pUser, s32 fd);

NWC24Err NWC24iIoctlResourceManager(const char* pUser, s32 fd, s32 type, void* pIn, s32 inSize, void* pOut, s32 outSize);

NWC24Err NWC24iIoctlResourceManagerAsync(const char* pUser, s32 fd, s32 type, void* pIn, s32 inSize, void* pOut, s32 outSize, void* pCallbackArg);

BOOL NWC24iIsAsyncRequestPending(void);

#define NWC24_OPEN_DEVICE(name, fdOut, mode) NWC24iOpenResourceManager(__FUNCTION__, name, fdOut, mode)

#define NWC24_CLOSE_DEVICE(fd) NWC24iCloseResourceManager(__FUNCTION__, fd)

#define NWC24_IOCTL_DEVICE(fd, type, in, inSize, out, outSize) NWC24iIoctlResourceManager(__FUNCTION__, fd, type, in, inSize, out, outSize)

#define NWC24_IOCTL_DEVICE_ASYNC(fd, type, in, inSize, out, outSize, callbackArg)                                                                    \
    NWC24iIoctlResourceManagerAsync(__FUNCTION__, fd, type, in, inSize, out, outSize, callbackArg)

#ifdef __cplusplus
}
#endif
#endif
