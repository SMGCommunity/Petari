#ifndef RVL_SDK_NWC24_SYSTEM_H
#define RVL_SDK_NWC24_SYSTEM_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

NWC24Err NWC24EnableLedNotification(BOOL enable);
BOOL NWC24Shutdown(BOOL final, u32 event);

#ifdef __cplusplus
}
#endif
#endif
