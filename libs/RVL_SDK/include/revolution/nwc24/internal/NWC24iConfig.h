#ifndef RVL_SDK_NWC24_INTERNAL_CONFIG_H
#define RVL_SDK_NWC24_INTERNAL_CONFIG_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct NWC24iConfig {
    u32 magic;
    u32 version;
    NWC24UserId userId;
    u32 createCount;
    u32 createStage;
    char acctDomain[64];
    char password[32];
    char mailchkId[36];
    char acctUrl[128];
    char mailchkUrl[128];
    char mailrecvUrl[128];
    char maildeleteUrl[128];
    char mailsendUrl[128];
    char UNK_0x31C[0x3F8 - 0x31C];
    BOOL allowTitleBoot;
    u32 checksum;
} NWC24iConfig;

NWC24Err NWC24iConfigOpen(void);
NWC24Err NWC24iConfigReload(void);
NWC24Err NWC24iConfigFlush(void);

#ifdef __cplusplus
}
#endif
#endif
