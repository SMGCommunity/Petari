#ifndef RVL_SDK_NWC24_INTERNAL_TIME_H
#define RVL_SDK_NWC24_INTERNAL_TIME_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

NWC24Err NWC24iGetUniversalTimeMinute(s32* minutes);
NWC24Err NWC24iGetUniversalTime(s64* pTime) NO_INLINE;
NWC24Err NWC24iGetTimeDifference(s64* pTimeDiff);
NWC24Err NWC24iSetRtcCounter(u32 rtc, u32 flags);
NWC24Err NWC24iSynchronizeRtcCounter(BOOL forceSave);

#ifdef __cplusplus
}
#endif
#endif
