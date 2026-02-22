#ifndef NWC24MBOXCTRL_H
#define NWC24MBOXCTRL_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

NWC24Err NWC24iOpenMBox(void);

typedef struct MountInfoStruct {
    u32 count;  // 0x00
    s32 type;   // 0x04
} MountInfoStruct;

typedef struct MBoxControlHeader {
    u32 magic;           // 0x00
    u32 version;         // 0x04
    u32 msgCount;        // 0x08
    u32 capacity;        // 0x0C
    u32 totalMsgSize;    // 0x10
    u32 mailDataOffset;  // 0x14
    u32 nextMsgId;       // 0x18
    u32 nextFreeEntry;   // 0x1C
    u32 oldestMsgId;     // 0x20
    u32 freeSpace;       // 0x24
    char padding[0x58];  // to 0x80
} MBoxControlHeader;

typedef struct MBoxControlEntry {
    u32 id;                            // 0x00
    u32 flags;                         // 0x04
    u32 length;                        // 0x08
    u32 appId;                         // 0x0C
    u32 UNK_0x10;                      // 0x10
    u32 tag;                           // 0x14
    u32 ledPattern;                    // 0x18
    u32 nextFreeOrCreationMs;          // 0x1C
    u64 fromId;                        // 0x20
    u32 createTime;                    // 0x28
    u32 UNK_0x2C;                      // 0x2C
    u8 numTo;                          // 0x30
    u8 numAttached;                    // 0x31
    u16 groupId;                       // 0x32
    u32 packedSubjectText;             // 0x34
    u32 packedTextSubjectSize;         // 0x38
    u32 packedSubjectTextSize;         // 0x3C
    u32 packedTextSizeContentType;     // 0x40
    u32 packedContentTypeTransferEnc;  // 0x44
    u32 textPtr;                       // 0x48
    u32 textSize;                      // 0x4C
    u32 attached0Ptr;                  // 0x50
    u32 attached0Size;                 // 0x54
    u32 attached1Ptr;                  // 0x58
    u32 attached1Size;                 // 0x5C
    u32 attached0OrigSize;             // 0x60
    u32 attached1OrigSize;             // 0x64
    u32 attached0_type;                // 0x68
    u32 attached1_type;                // 0x6C
    u32 textOrigSize;                  // 0x70
    u32 UNK_0x74;                      // 0x74
    u32 UNK_0x78;                      // 0x78
    u32 UNK_0x7C;                      // 0x7C
} MBoxControlEntry;

#ifdef __cplusplus
}
#endif

#endif  // NWC24MBOXCTRL_H
