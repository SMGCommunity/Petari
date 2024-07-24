#ifndef RVL_SDK_NWC24_MSG_OBJ_H
#define RVL_SDK_NWC24_MSG_OBJ_H
#include <revolution/nwc24/NWC24Types.h>
#include <revolution/nwc24/NWC24Utils.h>
#include <types.h>
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

#define NWC24_MSG_RECIPIENT_MAX 8
#define NWC24_MSG_ATTACHMENT_MAX 2

// Forward declarations
typedef struct RFLCharData;

typedef enum {
    NWC24_MSGTYPE_RVL_MENU_SHARED,

    NWC24_MSGTYPE_RVL,
    NWC24_MSGTYPE_RVL_APP = NWC24_MSGTYPE_RVL,

    NWC24_MSGTYPE_RVL_MENU,

    NWC24_MSGTYPE_RVL_HIDDEN,
    NWC24_MSGTYPE_RVL_APP_HIDDEN = NWC24_MSGTYPE_RVL_HIDDEN,

    NWC24_MSGTYPE_PUBLIC
} NWC24MsgType;

typedef struct NWC24MsgObj {
    u32 id;     // at 0x0
    u32 flags;  // at 0x4
    u32 length; // at 0x8
    u32 appId;  // at 0xC
    char UNK_0x10[0x4];
    u32 tag;        // at 0x14
    u32 ledPattern; // at 0x18
    u64 fromId;     // at 0x20
    u32 WORD_0x28;
    u32 WORD_0x2C;
    NWC24Data DATA_0x30;
    NWC24Data DATA_0x38;
    NWC24Data subject; // at 0x40
    NWC24Data text;    // at 0x48
    NWC24Data DATA_0x50;
    NWC24Data DATA_0x58;
    NWC24Charset charset;                                 // at 0x60
    NWC24Encoding encoding;                               // at 0x64
    NWC24Data attached[NWC24_MSG_ATTACHMENT_MAX];         // at 0x68
    u32 attachedSize[NWC24_MSG_ATTACHMENT_MAX];           // at 0x78
    NWC24MIMEType attachedType[NWC24_MSG_ATTACHMENT_MAX]; // at 0x80
    union {
        u64 toIds[NWC24_MSG_RECIPIENT_MAX];
        NWC24Data toAddrs[NWC24_MSG_RECIPIENT_MAX];
    };              // at 0x88
    u8 numTo;       // at 0xC8
    u8 numAttached; // at 0xC9
    u16 groupId;    // at 0xCA
    union {
        struct {
            u32 noreply : 1;
            u32 unknown : 7;
            u32 delay : 8;
            u32 regdate : 16;
        };

        u32 raw;
    } mb; // at 0xCC
    NWC24Data DATA_0xD0;
    NWC24Data face; // at 0xD8
    NWC24Data alt;  // at 0xE0
    char UNK_0xE8[0x100 - 0xE8];
} NWC24MsgObj;

NWC24Err NWC24InitMsgObj(NWC24MsgObj* msg, NWC24MsgType type);
NWC24Err NWC24SetMsgToId(NWC24MsgObj* msg, u64 id);
NWC24Err NWC24SetMsgText(NWC24MsgObj* msg, const char* text, u32 len,
                         NWC24Charset charset, NWC24Encoding encoding);
NWC24Err NWC24SetMsgFaceData(NWC24MsgObj* msg, const struct RFLCharData* data);
NWC24Err NWC24SetMsgAltName(NWC24MsgObj* msg, const wchar_t* name, u32 len);
NWC24Err NWC24SetMsgMBNoReply(NWC24MsgObj* msg, BOOL enable);
NWC24Err NWC24SetMsgMBRegDate(NWC24MsgObj* msg, u16 year, u8 month, u8 day);

#ifdef __cplusplus
}
#endif
#endif