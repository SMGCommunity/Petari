#ifndef NWC24MBOXCTRL_H
#define NWC24MBOXCTRL_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

NWC24Err NWC24iOpenMBox(void);

typedef struct MountInfoStruct {
    /* 0x0 */ u32 count;
    /* 0x4 */ s32 type;
} MountInfoStruct;

typedef struct MBoxControlHeader {
    /* 0x00 */ u32 magic;
    /* 0x04 */ u32 version;
    /* 0x08 */ u32 msgCount;
    /* 0x0C */ u32 capacity;
    /* 0x10 */ u32 totalMsgSize;
    /* 0x14 */ u32 mailDataOffset;
    /* 0x18 */ u32 nextMsgId;
    /* 0x1C */ u32 nextFreeEntry;
    /* 0x20 */ u32 oldestMsgId;
    /* 0x24 */ u32 freeSpace;
    /* 0x28 */ char padding[0x58];  // to 0x80
} MBoxControlHeader;

typedef struct MBoxControlEntry {
    /* 0x00 */ u32 id;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u32 length;
    /* 0x0C */ u32 appId;
    /* 0x10 */ u32 UNK_0x10;
    /* 0x14 */ u32 tag;
    /* 0x18 */ u32 ledPattern;
    /* 0x1C */ u32 nextFreeOrCreationMs;
    /* 0x20 */ u64 fromId;
    /* 0x28 */ u32 createTime;
    /* 0x2C */ u32 UNK_0x2C;
    /* 0x30 */ u8 numTo;
    /* 0x31 */ u8 numAttached;
    /* 0x32 */ u16 groupId;
    /* 0x34 */ u32 packedSubjectText;
    /* 0x38 */ u32 packedTextSubjectSize;
    /* 0x3C */ u32 packedSubjectTextSize;
    /* 0x40 */ u32 packedTextSizeContentType;
    /* 0x44 */ u32 packedContentTypeTransferEnc;
    /* 0x48 */ u32 textPtr;
    /* 0x4C */ u32 textSize;
    /* 0x50 */ u32 attached0Ptr;
    /* 0x54 */ u32 attached0Size;
    /* 0x58 */ u32 attached1Ptr;
    /* 0x5C */ u32 attached1Size;
    /* 0x60 */ u32 attached0OrigSize;
    /* 0x64 */ u32 attached1OrigSize;
    /* 0x68 */ u32 attached0_type;
    /* 0x6C */ u32 attached1_type;
    /* 0x70 */ u32 textOrigSize;
    /* 0x74 */ u32 UNK_0x74;
    /* 0x78 */ u32 UNK_0x78;
    /* 0x7C */ u32 UNK_0x7C;
} MBoxControlEntry;

#ifdef __cplusplus
}
#endif

#endif  // NWC24MBOXCTRL_H
