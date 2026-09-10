#ifndef RVL_SDK_NWC24_INTERNAL_SCHEDULE_H
#define RVL_SDK_NWC24_INTERNAL_SCHEDULE_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NWC24i_SCHEDULER_DEVICE "/dev/net/kd/request"

NWC24Err NWC24iRequestGenerateUserId(NWC24UserId* pUserId, u32* arg1);
NWC24Err NWC24iTrySuspendForOpenLib(void);
NWC24Err NWC24iResumeForCloseLib(void);

#ifdef __cplusplus
}
#endif
#endif
