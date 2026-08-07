#ifndef NWC24CONFIG_H
#define NWC24CONFIG_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { NWC24_IDCS_INITIAL, NWC24_IDCS_GENERATED, NWC24_IDCS_REGISTERED } NWC24IDCreationStage;

typedef struct NWC24MsgConfig {
    u32 magic;                             // at 0x0
    u32 version;                           // at 0x4
    u64 myUserId;                          // at 0x8
    u32 idCreationCount;                   // at 0x10
    NWC24IDCreationStage idCreationStage;  // at 0x14
    char domain[64];                       // at 0x18
    char password[32];                     // at 0x58
    char mailCheckId[36];                  // at 0x78
    char urlAMServer[128];                 // at 0x9C
    char urlMCServer[128];                 // at 0x11C
    char urlMRServer[128];                 // at 0x19C
    char urlMDServer[128];                 // at 0x21C
    char urlMTServer[128];                 // at 0x29C
    char optionProfile[128];               // at 0x31C
    char reserved[92];                     // at 0x39C
    u32 msgWakeupMode;                     // at 0x3F8
    u32 checkSum;                          // at 0x3FC
} NWC24MsgConfig;

NWC24Err NWC24GetMyUserId(u64* idOut);
NWC24Err NWC24GenerateNewUserId(u64* idOut);
NWC24Err NWC24iConfigOpen(void);
NWC24Err NWC24iConfigReload(void);
NWC24Err NWC24iConfigFlush(void);
char* NWC24GetAccountDomain(void);
char* NWC24GetMBoxDir(void);
u32 NWC24GetAppId(void);
u16 NWC24GetGroupId(void);

#ifdef __cplusplus
}
#endif

#endif  // NWC24CONFIG_H
