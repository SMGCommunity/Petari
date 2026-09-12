#ifndef RVL_SDK_NWC24_INTERNAL_MBOX_CTRL_H
#define RVL_SDK_NWC24_INTERNAL_MBOX_CTRL_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#include <revolution/nwc24/internal/NWC24iMsgObj.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NWC24i_MBOX_DIR_LENGTH 64
#define NWC24i_MBOX_MAX_SIZE 0x31C00

typedef struct NWC24iMBCHeader {
    u32 magic;
    u32 version;
    u32 numMsgs;
    u32 capacity;
    u32 totalMsgSize;
    u32 fileSize;
    u32 nextMsgId;
    u32 freeChain;
    u32 oldestMsgId;
    u32 freeSpace;
    u32 reserved[12];
    char lastUIDL[40];
} NWC24iMBCHeader;

typedef struct NWC24iMBCEntry {
    u32 id;
    u32 flags;
    u32 length;
    u32 appId;
    u32 unk10;
    u32 tag;
    u32 command;
    u32 unk1C;
    NWC24iAddr from;
    s32 createTime;
    u32 unk2C;
    u8 numTo;
    u8 numAttached;
    u16 groupId;
    u32 fromField;
    u32 toField;
    u32 subject;
    u32 contentType;
    u32 txEncoding;
    NWC24Data text;
    NWC24Data attached[NWC24i_MSG_ATTACHMENT_MAX];
    u32 attachedSize[NWC24i_MSG_ATTACHMENT_MAX];
    NWC24MIMEType attachedType[NWC24i_MSG_ATTACHMENT_MAX];
    NWC24Data dwcId;
    u32 unk78;
    u32 unk7C;
} NWC24iMBCEntry;

NWC24Err NWC24iOpenMBox(void);
NWC24Err NWC24iMBoxGetCtrlFilePath(NWC24MsgBoxId id, char* pDst, s32 maxlen);
NWC24Err NWC24iMBoxOpenNewMsg(NWC24MsgBoxId id, NWC24File* pFile, u32* pMsgID);
NWC24Err NWC24iMBoxCloseMsg(NWC24File* pFile);
NWC24Err NWC24iMBoxCancelMsg(NWC24File* pFile, NWC24MsgBoxId id, u32 msgId);
NWC24Err NWC24iMBoxAddMsgObj(NWC24MsgBoxId id, NWC24iMsgObj* pMsg);
NWC24Err NWC24iMBoxFlushHeader(NWC24MsgBoxId id);
NWC24Err NWC24iMBoxCheck(NWC24MsgBoxId id, u32 size);

#ifdef __cplusplus
}
#endif
#endif
