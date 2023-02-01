#ifndef ESP_H
#define ESP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <private/iosrestypes.h>
#include <private/iosresclt.h>

s32 IOS_Open(const char *, u32);
s32 IOS_Close(s32);

s32 IOS_IoctlvReboot(s32, s32, u32, u32, IOSIoVector *);

s32 IOS_IoctlAsync(s32, s32, void *, u32, void *, u32, IOSIpcCb, void *);

#ifdef __cplusplus
}
#endif

#endif // ESP_H