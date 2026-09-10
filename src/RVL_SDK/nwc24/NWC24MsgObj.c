#include <RVLFaceLib.h>

#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

#define MSG_YEAR_MIN 2000
#define MSG_YEAR_MAX 2035

#define ALT_NAME_MAX 35
#define SMTP_LINE_MAX 1000

NWC24Err NWC24InitMsgObj(NWC24MsgObj* pMsg, NWC24MsgType type) {
    u32 i;
    NWC24iMsgObj* pMsgImpl;

    pMsgImpl = (NWC24iMsgObj*)pMsg;
    Mail_memset(pMsgImpl, 0, sizeof(NWC24iMsgObj));

    pMsgImpl->id = 0;
    pMsgImpl->flags = NWC24_MSGOBJ_INITIALIZED;
    pMsgImpl->appId = NWC24GetAppId();
    pMsgImpl->command = 0;
    pMsgImpl->tag = 0;
    pMsgImpl->groupId = NWC24GetGroupId();

    switch (type) {
    case NWC24_MSGTYPE_RVL_MENU_SHARED: {
        pMsgImpl->flags |= NWC24_MSGOBJ_FOR_RECIPIENT | NWC24_MSGOBJ_FOR_APP | NWC24_MSGOBJ_FOR_MENU;
        break;
    }

    case NWC24_MSGTYPE_RVL_APP: {
        pMsgImpl->flags |= NWC24_MSGOBJ_FOR_RECIPIENT | NWC24_MSGOBJ_FOR_APP;
        break;
    }

    case NWC24_MSGTYPE_RVL_MENU: {
        pMsgImpl->flags |= NWC24_MSGOBJ_FOR_RECIPIENT | NWC24_MSGOBJ_FOR_MENU;
        break;
    }

    case NWC24_MSGTYPE_RVL_APP_HIDDEN: {
        pMsgImpl->flags |= NWC24_MSGOBJ_FOR_RECIPIENT;
        break;
    }

    case NWC24_MSGTYPE_PUBLIC: {
        pMsgImpl->flags |= NWC24_MSGOBJ_FOR_PUBLIC;
        break;
    }

    default: {
        return NWC24_ERR_INVALID_VALUE;
    }
    }

    pMsgImpl->createTime = 0;
    pMsgImpl->flags |= 1 << 21;
    pMsgImpl->unk2C = 0;
    NWC24GetMyUserId(&pMsgImpl->from.id);
    pMsgImpl->numTo = 0;

    for (i = 0; i < NWC24i_MSG_RECIPIENT_MAX; i++) {
        if (type == NWC24_MSGTYPE_PUBLIC) {
            NWC24Data_Init(&pMsgImpl->to[i].data);
        } else {
            pMsgImpl->to[i].id = 0;
        }
    }

    NWC24Data_Init(&pMsgImpl->subject);
    NWC24Data_Init(&pMsgImpl->text);
    pMsgImpl->charset = NWC24_US_ASCII;
    pMsgImpl->encoding = NWC24_ENC_7BIT;
    pMsgImpl->numAttached = 0;

    for (i = 0; i < NWC24i_MSG_ATTACHMENT_MAX; i++) {
        NWC24Data_Init(&pMsgImpl->attached[i]);
        pMsgImpl->attachedSize[i] = 0;
        pMsgImpl->attachedType[i] = NWC24_TXT_PLAIN;
    }

    NWC24Data_Init(&pMsgImpl->fromField);
    NWC24Data_Init(&pMsgImpl->toField);
    NWC24Data_Init(&pMsgImpl->contentType);
    NWC24Data_Init(&pMsgImpl->txEncoding);
    NWC24Data_Init(&pMsgImpl->user);
    NWC24Data_Init(&pMsgImpl->face);
    NWC24Data_Init(&pMsgImpl->alt);
    pMsgImpl->msgBoard = 0;
    pMsgImpl->dwcId.size = 0;
    pMsgImpl->iconNew = 0x80000000;

    return NWC24_OK;
}

NWC24Err NWC24SetMsgToId(NWC24MsgObj* pMsg, NWC24UserId userId) {
    NWC24iMsgObj* pMsgImpl = (NWC24iMsgObj*)pMsg;

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_INITIALIZED) || (pMsgImpl->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_FOR_RECIPIENT)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }

    if (pMsgImpl->numTo >= NWC24i_MSG_RECIPIENT_MAX) {
        return NWC24_ERR_FULL;
    }

    pMsgImpl->to[pMsgImpl->numTo].id = userId;
    pMsgImpl->numTo++;

    return NWC24_OK;
}

NWC24Err NWC24SetMsgText(NWC24MsgObj* pMsg, const char* pText, u32 len, NWC24Charset charset, NWC24Encoding encoding) {
    NWC24iMsgObj* pMsgImpl = (NWC24iMsgObj*)pMsg;

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_INITIALIZED) || (pMsgImpl->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (NWC24GetCharsetStr(charset) == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (NWC24GetEncodingStr(encoding) == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (pText == NULL) {
        NWC24Data_Init(&pMsgImpl->text);
        return NWC24_OK;
    }

    if (encoding == NWC24_ENC_8BIT) {
        if (pMsgImpl->flags & NWC24_MSGOBJ_FOR_PUBLIC) {
            return NWC24_ERR_NOT_SUPPORTED;
        }
    }

    if (encoding == NWC24_ENC_7BIT) {
        u32 lineLength;
        const char* pIt;
        const char* pEnd;

        lineLength = 0;
        pEnd = pText + len;

        for (pIt = pText; pIt < pEnd; pIt++) {
            if (pIt[0] == '\r' && pIt[1] == '\n') {
                lineLength = 0;
                pIt++;
            }

            else if (++lineLength > SMTP_LINE_MAX - 2) {
                return NWC24_ERR_FORMAT;
            }
        }
    }

    NWC24Data_SetDataP(&pMsgImpl->text, pText, len);
    pMsgImpl->charset = charset;
    pMsgImpl->encoding = encoding;

    return NWC24_OK;
}

NWC24Err NWC24SetMsgFaceData(NWC24MsgObj* pMsg, const RFLCharData* pData) {
    NWC24iMsgObj* pMsgImpl = (NWC24iMsgObj*)pMsg;

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_INITIALIZED) || (pMsgImpl->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (pMsgImpl->face.size > 0) {
        return NWC24_ERR_FULL;
    }

    NWC24Data_SetDataP(&pMsgImpl->face, pData, sizeof(RFLCharData));
    return NWC24_OK;
}

NWC24Err NWC24SetMsgAttached(NWC24MsgObj* pMsg, const char* data, u32 size, NWC24MIMEType type) {
    NWC24iMsgObj* obj = (NWC24iMsgObj*)pMsg;
    u32 total = 0;
    u32 maxSize;
    u32 i;
    if (!(obj->flags & NWC24_MSGOBJ_INITIALIZED) || (obj->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }
    if (data == NULL || size == 0) {
        return NWC24_ERR_NULL;
    }
    if (obj->numAttached >= NWC24i_MSG_ATTACHMENT_MAX) {
        return NWC24_ERR_FULL;
    }
    if (NWC24GetMIMETypeStr(type) == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }
    if (obj->flags & NWC24_MSGOBJ_FOR_PUBLIC) {
        if (type != 0x10000 && type != 0x10001 && type != 0x20000 && type != 0x30000) {
            return NWC24_ERR_NOT_SUPPORTED;
        }
    }
    switch (type) {
    case 0x20001:
        maxSize = 0x14000;
        break;
    case 0x30001:
        maxSize = 0x1E000;
        break;
    case 0x30002:
        maxSize = 0x80;
        break;
    default:
        maxSize = 0x245B0;
        break;
    }
    if (size > maxSize) {
        return NWC24_ERR_OVERFLOW;
    }
    for (i = 0; i < obj->numAttached; ++i) {
        total += obj->attachedSize[i];
    }
    total += size;
    if (total >= 0x245B0) {
        return NWC24_ERR_OVERFLOW;
    }
    NWC24Data_SetDataP(&obj->attached[obj->numAttached], data, size);
    obj->attachedSize[obj->numAttached] = size;
    obj->attachedType[obj->numAttached] = type;
    obj->numAttached++;
    obj->flags |= NWC24_MSGOBJ_MULTIPART;
    return NWC24_OK;
}

NWC24Err NWC24SetMsgTag(NWC24MsgObj* pMsg, u16 tag) {
    NWC24iMsgObj* obj = (NWC24iMsgObj*)pMsg;
    if (!(obj->flags & NWC24_MSGOBJ_INITIALIZED) || (obj->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }
    if (!(obj->flags & NWC24_MSGOBJ_FOR_RECIPIENT)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }
    obj->tag = obj->tag & 0xFFFF0000 | tag;
    return NWC24_OK;
}

NWC24Err NWC24SetMsgAltName(NWC24MsgObj* pMsg, const u16* pName, u32 len) {
    NWC24iMsgObj* pMsgImpl = (NWC24iMsgObj*)pMsg;

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_INITIALIZED) || (pMsgImpl->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (pMsgImpl->alt.size > 0) {
        return NWC24_ERR_FULL;
    }

    if (len > ALT_NAME_MAX) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (pName == NULL || len <= 0) {
        return NWC24_ERR_NULL;
    }

    NWC24Data_SetDataP(&pMsgImpl->alt, pName, len * sizeof(u16));
    return NWC24_OK;
}

NWC24Err NWC24SetMsgMBNoReply(NWC24MsgObj* pMsg, BOOL enable) {
    NWC24iMsgObj* pMsgImpl = (NWC24iMsgObj*)pMsg;

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_INITIALIZED) || (pMsgImpl->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_FOR_MENU)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }

    if (enable) {
        NWC24i_MSGOBJ_SET_MB_NOREPLY(pMsgImpl, TRUE);
    } else {
        NWC24i_MSGOBJ_SET_MB_NOREPLY(pMsgImpl, FALSE);
    }

    return NWC24_OK;
}

NWC24Err NWC24SetMsgMBRegDate(NWC24MsgObj* pMsg, u16 year, u8 month, u8 day) {
    NWC24iMsgObj* pMsgImpl;
    NWC24Err result;
    u16 regdate;

    pMsgImpl = (NWC24iMsgObj*)pMsg;

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_INITIALIZED) || (pMsgImpl->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_FOR_MENU)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }

    if (year < MSG_YEAR_MIN || year > MSG_YEAR_MAX) {
        return NWC24_ERR_INVALID_VALUE;
    }

    result = NWC24iIsValidDate(year, month, day);
    if (result != NWC24_OK) {
        return result;
    }

    regdate = (year - MSG_YEAR_MIN) << 9 | month << 5 | day;
    NWC24i_MSGOBJ_SET_MB_REGDATE(pMsgImpl, regdate);

    return NWC24_OK;
}

NWC24Err NWC24SetMsgMBDelay(NWC24MsgObj* pMsg, u8 delay) {
    NWC24iMsgObj* obj = (NWC24iMsgObj*)pMsg;
    NWC24UserId myId;
    NWC24Err result;
    if (!(obj->flags & NWC24_MSGOBJ_INITIALIZED) || (obj->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }
    if (!(obj->flags & NWC24_MSGOBJ_FOR_MENU)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }
    result = NWC24GetMyUserId(&myId);
    if (result < 0) {
        return result;
    }
    if (obj->numTo != 1 || obj->to[0].id != myId) {
        return NWC24_ERR_NOT_SUPPORTED;
    }
    NWC24i_MSGOBJ_SET_MB_DELAY(obj, delay << 16);
    return NWC24_OK;
}

NWC24Err NWC24SetMsgLedPattern(NWC24MsgObj* pMsg, u16 pattern) {
    NWC24iMsgObj* obj = (NWC24iMsgObj*)pMsg;
    if (!(obj->flags & NWC24_MSGOBJ_INITIALIZED) || (obj->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }
    if (pattern == 0) {
        return NWC24_ERR_INVALID_VALUE;
    }
    if (!(obj->flags & NWC24_MSGOBJ_FOR_RECIPIENT) || !(obj->flags & NWC24_MSGOBJ_FOR_MENU)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }
    if (obj->command != 0 && !(obj->command & 0x40000)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }
    if (!NWC24IsMsgLibOpenedByTool() && NWC24GetAppId() != 0x48414541) {
        if (pattern >= 0x4000) {
            return NWC24_ERR_INVALID_VALUE;
        }
    }
    obj->command = pattern | 0x40000;
    return NWC24_OK;
}

NWC24Err NWC24GetMsgSize(const NWC24MsgObj* pMsg, u32* size) {
    const NWC24iMsgObj* obj = (const NWC24iMsgObj*)pMsg;
    if (!(obj->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }
    *size = obj->length;
    return NWC24_OK;
}

NWC24Err NWC24SetMsgDesignatedTime(NWC24MsgObj* pMsg, s32 delay) {
    NWC24iMsgObj* obj = (NWC24iMsgObj*)pMsg;
    s32 minutes = 0;
    s32 designated;
    NWC24Err result;
    if (delay <= 0 || delay > 14400) {
        return NWC24_ERR_INVALID_VALUE;
    }
    result = NWC24iGetUniversalTimeMinute(&minutes);
    if (result < 0) {
        return result;
    }
    designated = minutes + delay;
    if (!(obj->flags & NWC24_MSGOBJ_INITIALIZED) || (obj->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }
    obj->unk2C = designated;
    obj->flags |= 1 << 24;
    return NWC24_OK;
}
