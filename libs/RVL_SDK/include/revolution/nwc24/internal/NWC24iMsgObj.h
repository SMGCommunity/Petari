#ifndef RVL_SDK_NWC24_INTERNAL_MSG_OBJ_H
#define RVL_SDK_NWC24_INTERNAL_MSG_OBJ_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#include <revolution/nwc24/NWC24Utils.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NWC24i_MSG_RECIPIENT_MAX 8
#define NWC24i_MSG_ATTACHMENT_MAX 2

#define NWC24i_WII_ID_LEN 16

#define NWC24i_APP_ID_LEN 15

#define NWC24i_MSG_ID_LEN 30

#define NWC24i_MSGOBJ_GET_MB_NOREPLY(pObj) ((pObj)->msgBoard & 0x80000000)
#define NWC24i_MSGOBJ_SET_MB_NOREPLY(pObj, x) ((x) ? ((pObj)->msgBoard |= 0x80000000) : ((pObj)->msgBoard &= ~0x80000000))

#define NWC24i_MSGOBJ_GET_MB_DELAY(pObj) ((pObj)->msgBoard & 0x00FF0000)
#define NWC24i_MSGOBJ_SET_MB_DELAY(pObj, x) ((pObj)->msgBoard = ((pObj)->msgBoard & ~0x00FF0000) | ((x) & 0x00FF0000))

#define NWC24i_MSGOBJ_GET_MB_REGDATE(pObj) ((pObj)->msgBoard & 0x0000FFFF)
#define NWC24i_MSGOBJ_SET_MB_REGDATE(pObj, x) ((pObj)->msgBoard = ((pObj)->msgBoard & ~0x0000FFFF) | ((x) & 0x0000FFFF))

typedef enum NWC24iMsgObjFlags {
    NWC24_MSGOBJ_FOR_RECIPIENT = 1 << 0,
    NWC24_MSGOBJ_FOR_PUBLIC = 1 << 1,
    NWC24_MSGOBJ_FOR_APP = 1 << 2,
    NWC24_MSGOBJ_FOR_MENU = 1 << 3,
    NWC24_MSGOBJ_TO_SEND = 1 << 4,
    NWC24_MSGOBJ_TO_RECV = 1 << 5,
    NWC24_MSGOBJ_INITIALIZED = 1 << 8,
    NWC24_MSGOBJ_DELIVERING = 1 << 9,
    NWC24_MSGOBJ_DWC = 1 << 13,
    NWC24_MSGOBJ_MULTIPART = 1 << 16,
    NWC24_MSGOBJ_FLAG_20 = 1 << 20,
} NWC24iMsgObjFlags;

typedef union NWC24iAddr {
    NWC24UserId id;
    NWC24Data data;
} NWC24iAddr;

typedef struct NWC24iMsgObj {
    u32 id;
    u32 flags;
    u32 length;
    u32 appId;
    s32 unk10;
    u32 tag;
    u32 command;
    u32 unk1C;
    NWC24iAddr from;
    s32 createTime;
    u32 unk2C;
    NWC24Data fromField;
    NWC24Data toField;
    NWC24Data subject;
    NWC24Data text;
    NWC24Data contentType;
    NWC24Data txEncoding;
    NWC24Charset charset;
    NWC24Encoding encoding;
    NWC24Data attached[NWC24i_MSG_ATTACHMENT_MAX];
    u32 attachedSize[NWC24i_MSG_ATTACHMENT_MAX];
    NWC24MIMEType attachedType[NWC24i_MSG_ATTACHMENT_MAX];
    union {
        NWC24iAddr to[NWC24i_MSG_RECIPIENT_MAX];
    };
    u8 numTo;
    u8 numAttached;
    u16 groupId;
    u32 msgBoard;
    NWC24Data user;
    NWC24Data face;
    NWC24Data alt;
    NWC24Data dwcId;
    u32 iconNew;
    char reserved[0x100 - 0xF4];
} NWC24iMsgObj;

#ifdef __cplusplus
}
#endif
#endif
