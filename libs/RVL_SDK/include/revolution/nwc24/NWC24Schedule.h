#ifndef NWC24SCHEDULE_H
#define NWC24SCHEDULE_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

s32 NWC24SuspendScheduler(void);
s32 NWC24ResumeScheduler(void);
NWC24Err NWC24iRequestGenerateUserId(u64* idOut, u32* arg1);
NWC24Err NWC24iTrySuspendForOpenLib(void);
NWC24Err NWC24iResumeForCloseLib(void);

#ifdef __cplusplus
}
#endif

#endif  // NWC24SCHEDULE_H
