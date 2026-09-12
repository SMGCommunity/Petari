#ifndef RVL_SDK_NWC24_INTERNAL_SYSTEM_H
#define RVL_SDK_NWC24_INTERNAL_SYSTEM_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

NWC24Err NWC24iPrepareShutdown(void);
NWC24Err NWC24iRequestShutdown(u32 event, NWC24Err* pResult);

#ifdef __cplusplus
}
#endif
#endif
