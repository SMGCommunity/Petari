#ifndef NWC24CONFIG_H
#define NWC24CONFIG_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { NWC24_IDCS_INITIAL, NWC24_IDCS_GENERATED, NWC24_IDCS_REGISTERED } NWC24IDCreationStage;

typedef struct NWC24Config {
    u32 magic;                         // at 0x0
    u32 version;                       // at 0x4
    u64 userId;                        // at 0x8
    u32 createCount;                   // at 0x10
    NWC24IDCreationStage createStage;  // at 0x14
    char acctDomain[64];               // at 0x18
    char password[32];                 // at 0x58
    char mailchkId[36];                // at 0x78
    char acctUrl[128];                 // at 0x9C
    char mailchkUrl[128];              // at 0x11C
    char mailrecvUrl[128];             // at 0x19C
    char maildeleteUrl[128];           // at 0x21C
    char mailsendUrl[128];             // at 0x29C
    char UNK_0x31C[0x3F8 - 0x31C];
    BOOL allowTitleBoot;  // at 0x3F8
    u32 checksum;         // at 0x3FC
} NWC24Config;

NWC24Err NWC24GetMyUserId(u64* idOut);
NWC24Err NWC24GenerateNewUserId(u64* idOut);
NWC24Err NWC24iConfigOpen(void);
NWC24Err NWC24iConfigReload(void);
NWC24Err NWC24iConfigFlush(void);
const char* NWC24GetAccountDomain(void);
const char* NWC24GetMBoxDir(void);
u32 NWC24GetAppId(void);
u16 NWC24GetGroupId(void);

#ifdef __cplusplus
}
#endif

#endif  // NWC24CONFIG_H
