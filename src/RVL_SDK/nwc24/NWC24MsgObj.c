#include "revolution/nwc24.h"
#include "revolution/nwc24/NWC24Config.h"
#include "revolution/nwc24/NWC24Parser.h"
#include "revolution/nwc24/NWC24StdApi.h"
#include "revolution/nwc24/NWC24Types.h"

#define YEAR_MIN 2000
#define YEAR_MAX 2035  // !!!

#define ALT_NAME_MAX 35
#define SMTP_LINE_MAX 1000

typedef enum {
    MSG_OBJ_FOR_RECIPIENT = (1 << 0),
    MSG_OBJ_FOR_PUBLIC = (1 << 1),
    MSG_OBJ_FOR_APP = (1 << 2),
    MSG_OBJ_FOR_MENU = (1 << 3),
    MSG_OBJ_INITIALIZED = (1 << 8),
    MSG_OBJ_DELIVERING = (1 << 9)
} NWC24MsgObjFlags;

NWC24Err NWC24InitMsgObj(NWC24MsgObj* msg, NWC24MsgType type) {
    u32 i;
    NWC24MsgObj* _msg = (NWC24MsgObj*)msg;

    Mail_memset(_msg, 0, sizeof(NWC24MsgObj));

    _msg->id = 0;
    _msg->flags = MSG_OBJ_INITIALIZED;
    _msg->appId = NWC24GetAppId();
    _msg->ledPattern = 0;
    _msg->tag = 0;
    _msg->groupId = NWC24GetGroupId();

    switch (type) {
    case NWC24_MSGTYPE_RVL_MENU_SHARED:
        _msg->flags |= MSG_OBJ_FOR_RECIPIENT | MSG_OBJ_FOR_APP | MSG_OBJ_FOR_MENU;
        break;
    case NWC24_MSGTYPE_RVL:
        _msg->flags |= MSG_OBJ_FOR_RECIPIENT | MSG_OBJ_FOR_APP;
        break;
    case NWC24_MSGTYPE_RVL_MENU:
        _msg->flags |= MSG_OBJ_FOR_RECIPIENT | MSG_OBJ_FOR_MENU;
        break;
    case NWC24_MSGTYPE_RVL_HIDDEN:
        _msg->flags |= MSG_OBJ_FOR_RECIPIENT;
        break;
    case NWC24_MSGTYPE_PUBLIC:
        _msg->flags |= MSG_OBJ_FOR_PUBLIC;
        break;
    default:
        return NWC24_ERR_INVALID_VALUE;
    }

    _msg->flags |= 0x200000;
    _msg->WORD_0x28 = 0;
    _msg->WORD_0x2C = 0;
    NWC24GetMyUserId(&_msg->fromId);
    _msg->numTo = 0;

    for (i = 0; i < NWC24_MSG_RECIPIENT_MAX; i++) {
        if (type == NWC24_MSGTYPE_PUBLIC) {
            NWC24Data_Init(&_msg->toAddrs[i]);
        } else {
            _msg->toIds[i] = 0;
        }
    }

    NWC24Data_Init(&_msg->subject);
    NWC24Data_Init(&_msg->text);
    _msg->charset = NWC24_US_ASCII;
    _msg->encoding = NWC24_ENC_7BIT;
    _msg->numAttached = 0;

    for (i = 0; i < NWC24_MSG_ATTACHMENT_MAX; i++) {
        NWC24Data_Init(&_msg->attached[i]);
        _msg->attachedSize[i] = 0;
        _msg->attachedType[i] = NWC24_TEXT_PLAIN;
    }

    NWC24Data_Init(&_msg->DATA_0x30);
    NWC24Data_Init(&_msg->DATA_0x38);
    NWC24Data_Init(&_msg->DATA_0x50);
    NWC24Data_Init(&_msg->DATA_0x58);
    NWC24Data_Init(&_msg->DATA_0xD0);
    NWC24Data_Init(&_msg->face);
    NWC24Data_Init(&_msg->alt);
    _msg->mb.raw = 0;

    // NEW IN SMG1
    _msg->_EC = 0;
    _msg->_F0 = 0x80000000;

    return NWC24_OK;
}

NWC24Err NWC24SetMsgToId(NWC24MsgObj* msg, u64 id) {
    if (!(msg->flags & MSG_OBJ_INITIALIZED) || (msg->flags & MSG_OBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (!(msg->flags & MSG_OBJ_FOR_RECIPIENT)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }

    if (msg->numTo >= NWC24_MSG_RECIPIENT_MAX) {
        return NWC24_ERR_FULL;
    }

    msg->toIds[msg->numTo] = id;
    msg->numTo++;

    return NWC24_OK;
}

NWC24Err NWC24SetMsgText(NWC24MsgObj* msg, const char* text, u32 len, NWC24Charset charset, NWC24Encoding encoding) {
    if (!(msg->flags & MSG_OBJ_INITIALIZED) || (msg->flags & MSG_OBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (NWC24GetCharsetStr(charset) == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (NWC24GetEncodingStr(encoding) == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (text == NULL) {
        NWC24Data_Init(&msg->text);
        return NWC24_OK;
    }

    // NEW IN SMG1
    if (encoding == NWC24_ENC_8BIT) {
        if (msg->flags & 0x2) {
            return NWC24_ERR_NOT_SUPPORTED;
        }
    }

    if (encoding == NWC24_ENC_7BIT) {
        const char* it;
        u32 lineLength;

        lineLength = 0;

        for (it = text; it < text + len; it++) {
            if (it[0] == '\r' && it[1] == '\n') {
                lineLength = 0;
                it++;
            }
            // Include "\r\n" in line length
            else if (++lineLength > SMTP_LINE_MAX - 2) {
                return NWC24_ERR_FORMAT;
            }
        }
    }

    NWC24Data_SetDataP(&msg->text, text, len);
    msg->charset = charset;
    msg->encoding = encoding;
    return NWC24_OK;
}

// NWC24SetMsgAttached

NWC24Err NWC24SetMsgTag(NWC24MsgObj* obj, u16 tag) {
    if ((obj->flags & 0x100) == 0 || (obj->flags & 0x200) != 0) {
        return NWC24_ERR_PROTECTED;
    }

    if ((obj->flags & 0x1) == 0) {
        return NWC24_ERR_NOT_SUPPORTED;
    }

    obj->tag = obj->tag & 0xFFFF0000 | tag;
    return NWC24_OK;
}

NWC24Err NWC24SetMsgAltName(NWC24MsgObj* msg, const wchar_t* name, u32 len) {
    if (!(msg->flags & MSG_OBJ_INITIALIZED) || (msg->flags & MSG_OBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (msg->alt.size > 0) {
        return NWC24_ERR_FULL;
    }

    if (len > ALT_NAME_MAX) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (name == NULL || len == 0) {
        return NWC24_ERR_NULL;
    }

    NWC24Data_SetDataP(&msg->alt, name, len * sizeof(wchar_t));
    return NWC24_OK;
}

NWC24Err NWC24SetMsgMBNoReply(NWC24MsgObj* msg, BOOL enable) {
    if (!(msg->flags & MSG_OBJ_INITIALIZED) || (msg->flags & MSG_OBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (!(msg->flags & MSG_OBJ_FOR_MENU)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }

    if (enable) {
        msg->mb.noreply = TRUE;
    } else {
        msg->mb.noreply = FALSE;
    }

    return NWC24_OK;
}

// NWC24SetMsgMBDelay
// NWC24SetMsgLedPattern

NWC24Err NWC24GetMsgSize(const NWC24MsgObj* obj, u32* size) {
    if ((obj->flags & 0x200) == 0) {
        return NWC24_ERR_PROTECTED;
    }

    *size = obj->length;
    return NWC24_OK;
}

// NWC24SetMsgDesignatedTime
