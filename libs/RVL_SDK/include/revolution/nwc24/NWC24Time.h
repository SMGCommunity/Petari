#ifndef NWC24TIME_H
#define NWC24TIME_H

#include "revolution/nwc24.h"
#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

NWC24Err NWC24iGetUniversalTime(s64* timeOut);
NWC24Err NWC24iGetTimeDifference(s64* diffOut);
NWC24Err NWC24iSetRtcCounter(u32 rtc, u32 flags);
NWC24Err NWC24iSynchronizeRtcCounter(BOOL forceSave);

#ifdef __cplusplus
}
#endif

#endif  // NWC24TIME_H
