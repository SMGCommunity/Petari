#ifndef NWC24SECRETFLIST_H
#define NWC24SECRETFLIST_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NWC24SecretFLHeader {
    u32 magic;    // at 0x0
    u32 version;  // at 0x4
    u8 UNK_0x8[0x800 - 0x8];
} NWC24SecretFLHeader;

NWC24Err NWC24iOpenSecretFriendList(void);

#ifdef __cplusplus
}
#endif

#endif  // NWC24SECRETFLIST_H
