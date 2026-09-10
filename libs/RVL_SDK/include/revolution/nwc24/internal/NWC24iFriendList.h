#ifndef RVL_SDK_NWC24_INTERNAL_FRIEND_LIST_H
#define RVL_SDK_NWC24_INTERNAL_FRIEND_LIST_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct NWC24iFLHeader {
    u32 magic;
    u32 version;
    u32 capacity;
    u32 size;
    char unk10[0x40 - 0x10];
    u64 friendCodes[NWC24_FRIEND_LIST_CAPACITY];
} NWC24iFLHeader;

NWC24Err NWC24iOpenFriendList(void);

#ifdef __cplusplus
}
#endif
#endif
