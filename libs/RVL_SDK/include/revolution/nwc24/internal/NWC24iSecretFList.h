#ifndef RVL_SDK_NWC24_INTERNAL_SECRET_FRIEND_LIST_H
#define RVL_SDK_NWC24_INTERNAL_SECRET_FRIEND_LIST_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct NWC24iSecretFLHeader {
    u32 magic;
    u32 version;
    u8 UNK_0x8[0x800 - 0x8];
} NWC24iSecretFLHeader;

NWC24Err NWC24iOpenSecretFriendList(void);

#ifdef __cplusplus
}
#endif
#endif
