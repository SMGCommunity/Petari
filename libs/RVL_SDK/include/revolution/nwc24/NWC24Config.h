#ifndef RVL_SDK_NWC24_CONFIG_H
#define RVL_SDK_NWC24_CONFIG_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum { NWC24_IDCS_INITIAL, NWC24_IDCS_GENERATED, NWC24_IDCS_REGISTERED } NWC24IDCreationStage;

NWC24Err NWC24GetMyUserId(u64* pId);
NWC24Err NWC24GenerateNewUserId(u64* pId);
const char* NWC24GetAccountDomain(void);
const char* NWC24GetMBoxDir(void);
u32 NWC24GetAppId(void);
u16 NWC24GetGroupId(void);

#ifdef __cplusplus
}
#endif
#endif
