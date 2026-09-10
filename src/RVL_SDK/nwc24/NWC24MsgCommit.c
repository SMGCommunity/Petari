#include <revolution/net.h>
#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>
#include <revolution/os.h>

#define B64_STEP (60 - (NWC24i_KSTRLEN(" ") - 1 + NWC24i_KSTRLEN("\r\n") - 1))

#define B64_ENCODE_SIZE(X) ((X) * 4 / 3)
#define B64_DECODE_SIZE(X) ((X) * 3 / 4)

#define MSG_WORK_SIZE (NWC24i_STRING_WORK_SIZE - (NWC24i_KSTRLEN("\r\n") - 1))

static NWC24File* m_pFile = NULL;
static BOOL LoopBackEnable = TRUE;

#define MULTIPART_DIVIDER_LEN (NWC24i_KSTRLEN("Boundary-NWC24-%08X%05X") + 8 + 5 + NWC24i_KSTRLEN("\r\n"))

static char MultiPartDivider[MULTIPART_DIVIDER_LEN] = "";

static NWC24Err CorrectHeadersForMBDelay(NWC24iMsgObj* pMsg) NO_INLINE;
static NWC24Err CheckMsgObject(const NWC24iMsgObj* pMsg);
static NWC24Err CheckMsgBoxSpace(const NWC24iMsgObj* pMsg, NWC24MsgBoxId id);

static NWC24Err SynthesizeAddrStr(const NWC24iAddr* addrId, u32 flags, char* pBuffer, int bufferSize, u32* pWritten);

static NWC24Err WriteSMTP_MAILFROM(NWC24iMsgObj* pMsg);
static NWC24Err WriteSMTP_RCPTTO(NWC24iMsgObj* pMsg);
static NWC24Err WriteSMTP_DATA(NWC24iMsgObj* pMsg);
static NWC24Err WriteFromField(NWC24iMsgObj* pMsg);
static NWC24Err WriteToField(NWC24iMsgObj* pMsg);
static NWC24Err WriteSubjectField(NWC24iMsgObj* pMsg);
static NWC24Err WriteDateField(NWC24iMsgObj* pMsg);
static NWC24Err WriteMessageIdField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiAppIdField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiTagField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiCmdField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiDWCIdField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiFaceField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiIconNewField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiAltNameField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiMBNoReplyField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiMBRegDateField(NWC24iMsgObj* pMsg);
static NWC24Err WriteXWiiMBDelayField(NWC24iMsgObj* pMsg);
static NWC24Err WriteMIMETopHeader(NWC24iMsgObj* pMsg);
static NWC24Err WriteMIMEPartBoundary(NWC24iMsgObj* pMsg);
static NWC24Err WriteMIMEAttachHeader(NWC24iMsgObj* pMsg, s32 idx);
static NWC24Err WriteContentTypeField(NWC24iMsgObj* pMsg);
static NWC24Err WriteUserField(NWC24iMsgObj* pMsg);
static NWC24Err WritePlainText(NWC24iMsgObj* pMsg);
static NWC24Err WriteAttachData(NWC24iMsgObj* pMsg, s32 idx);
static NWC24Err WriteBase64Data(const void* pData, s32 size, s32* pWritten);
static NWC24Err WriteQPData(const void* pData, s32 size, s32* pWritten);

static NWC24Err WriteTextBody(NWC24iMsgObj* pMsg);
static NWC24Err WriteMIMEPartBoundaryEnd(NWC24iMsgObj* pMsg);

NWC24Err NWC24CommitMsg(NWC24MsgObj* pMsg) {
    NWC24MsgBoxId box;
    NWC24iMsgObj* pMsgImpl;

    box = NWC24_MSGBOX_SEND;
    pMsgImpl = (NWC24iMsgObj*)pMsg;

    if (!NWC24IsMsgLibOpened() && !NWC24IsMsgLibOpenedByTool()) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }

    if (!(pMsgImpl->flags & NWC24_MSGOBJ_INITIALIZED) || (pMsgImpl->flags & NWC24_MSGOBJ_DELIVERING)) {
        return NWC24_ERR_PROTECTED;
    }

    if (LoopBackEnable && (pMsgImpl->flags & NWC24_MSGOBJ_FOR_RECIPIENT)) {
        NWC24UserId myId;
        NWC24GetMyUserId(&myId);

        if (pMsgImpl->numTo == 1 && pMsgImpl->to[0].id == myId) {
            box = NWC24_MSGBOX_RECV;
        }
    }

    return NWC24CommitMsgInternal(pMsgImpl, box);
}

NWC24Err NWC24CommitMsgInternal(NWC24iMsgObj* pMsg, NWC24MsgBoxId id) {
    NWC24File file;
    s32 i = 0;
    u32 msgId;
    NWC24Data subject;
    NWC24Data text;
    NWC24Data attachment[NWC24i_MSG_ATTACHMENT_MAX];
    NWC24Err result;
    NWC24Err err;
    u32 arrivedFlags;

    NWC24Data_Init(&subject);
    NWC24Data_Init(&text);

    for (i = 0; i < NWC24i_MSG_ATTACHMENT_MAX; i++) {
        NWC24Data_Init(&attachment[i]);
    }

    NWC24iSetErrorCode(NWC24_OK);

    err = CorrectHeadersForMBDelay(pMsg);
    if (err != NWC24_OK) {
        goto _exit;
    }

    err = CheckMsgObject(pMsg);
    if (err != NWC24_OK) {
        goto _exit;
    }

    err = CheckMsgBoxSpace(pMsg, id);
    if (err != NWC24_OK) {
        goto _exit;
    }

    result = NWC24iMBoxOpenNewMsg(id, &file, &msgId);
    if (result != NWC24_OK) {
        err = result;
        goto _error_break;
    }

    pMsg->id = msgId;
    pMsg->length = 0;

    m_pFile = &file;

#define HANDLE_ERROR(X)                                                                                                                              \
    if ((X) != NWC24_OK) {                                                                                                                           \
        NWC24FClose(&file);                                                                                                                          \
        err = result;                                                                                                                                \
        goto _error_break;                                                                                                                           \
    }

    if (id == NWC24_MSGBOX_SEND) {
        result = WriteSMTP_MAILFROM(pMsg);
        HANDLE_ERROR(result);

        result = WriteSMTP_RCPTTO(pMsg);
        HANDLE_ERROR(result);

        result = WriteSMTP_DATA(pMsg);
        HANDLE_ERROR(result);
    }

    result = WriteDateField(pMsg);
    HANDLE_ERROR(result);

    result = WriteFromField(pMsg);
    HANDLE_ERROR(result);

    result = WriteToField(pMsg);
    HANDLE_ERROR(result);

    result = WriteMessageIdField(pMsg);
    HANDLE_ERROR(result);

    subject.offset = pMsg->length + NWC24i_KSTRLEN("Subject: ") - 1;
    result = WriteSubjectField(pMsg);
    if (result == NWC24_OK) {
        subject.size = pMsg->length - subject.offset - 2;
    } else if (result == NWC24_ERR_NULL) {
        result = NWC24_OK;
        subject.pData = 0;
    }

    HANDLE_ERROR(result);

    if (pMsg->flags & NWC24_MSGOBJ_FOR_RECIPIENT) {
        result = WriteXWiiAppIdField(pMsg);
        HANDLE_ERROR(result);

        result = WriteXWiiCmdField(pMsg);
        HANDLE_ERROR(result);

        result = WriteXWiiTagField(pMsg);
        HANDLE_ERROR(result);

        result = WriteXWiiDWCIdField(pMsg);
        HANDLE_ERROR(result);

        result = WriteXWiiAltNameField(pMsg);
        HANDLE_ERROR(result);

        result = WriteXWiiFaceField(pMsg);
        HANDLE_ERROR(result);

        result = WriteXWiiIconNewField(pMsg);
        HANDLE_ERROR(result);

        if (pMsg->msgBoard != 0) {
            result = WriteXWiiMBNoReplyField(pMsg);
            HANDLE_ERROR(result);

            result = WriteXWiiMBRegDateField(pMsg);
            HANDLE_ERROR(result);

            result = WriteXWiiMBDelayField(pMsg);
            HANDLE_ERROR(result);
        }
    }

    result = WriteUserField(pMsg);
    HANDLE_ERROR(result);

    Mail_memset(MultiPartDivider, 0, MULTIPART_DIVIDER_LEN);
    Mail_sprintf(MultiPartDivider, "Boundary-NWC24-%08X%05X", pMsg->createTime, pMsg->id);

    result = WriteMIMETopHeader(pMsg);
    HANDLE_ERROR(result);

    if (pMsg->flags & NWC24_MSGOBJ_MULTIPART) {
        pMsg->unk10 = pMsg->length;

        result = WriteMIMEPartBoundary(pMsg);
        HANDLE_ERROR(result);
    }

    result = WriteContentTypeField(pMsg);
    HANDLE_ERROR(result);

    if (!(pMsg->flags & NWC24_MSGOBJ_MULTIPART)) {
        pMsg->unk10 = pMsg->length;
    }

    text.offset = pMsg->length;
    result = WriteTextBody(pMsg);
    HANDLE_ERROR(result);
    text.size = pMsg->length - text.offset;

    for (i = 0; i < (u32)pMsg->numAttached; i++) {
        result = WriteMIMEPartBoundary(pMsg);
        HANDLE_ERROR(result);

        result = WriteMIMEAttachHeader(pMsg, i);
        HANDLE_ERROR(result);

        attachment[i].offset = pMsg->length;
        result = WriteAttachData(pMsg, i);
        HANDLE_ERROR(result);
        attachment[i].size = pMsg->length - attachment[i].offset;

        if (i == pMsg->numAttached - 1) {
            result = WriteMIMEPartBoundaryEnd(pMsg);
            HANDLE_ERROR(result);
        }
    }

_error_break:
    if (err == NWC24_OK) {
        NWC24iMBoxCloseMsg(&file);
    } else {
        NWC24iMBoxCancelMsg(&file, id, msgId);
        NWC24iMBoxFlushHeader(id);
        goto _exit;
    }

    pMsg->subject = subject;
    pMsg->text = text;
    for (i = 0; i < NWC24i_MSG_ATTACHMENT_MAX; i++) {
        pMsg->attached[i] = attachment[i];
    }
    pMsg->flags |= NWC24_MSGOBJ_DELIVERING;

    if (id == NWC24_MSGBOX_RECV) {
        pMsg->flags |= NWC24_MSGOBJ_TO_RECV;
    } else if (id == NWC24_MSGBOX_SEND) {
        pMsg->flags |= NWC24_MSGOBJ_TO_SEND;
    }

    result = NWC24iMBoxAddMsgObj(id, pMsg);

    if (err != NWC24_OK) {
        result = err;
    }

    err = result;

    if (id == NWC24_MSGBOX_RECV) {
        arrivedFlags = NWC24_MSG_ARRIVED;

        if (pMsg->flags & NWC24_MSGOBJ_FOR_MENU) {
            arrivedFlags |= NWC24_MSG_ARRIVED_FOR_MENU;
        }

        NWC24iSetNewMsgArrived(arrivedFlags);
    }

_exit:
    if (err == NWC24_ERR_FULL || (err <= NWC24_ERR_FILE_OPEN && err >= NWC24_ERR_FILE_OTHER) || err == NWC24_ERR_NAND_CORRUPT ||
        err == NWC24_ERR_FILE_EXISTS || err == NWC24_ERR_INTERNAL_VF || err == NWC24_ERR_FILE_BROKEN) {
        NWC24iSetErrorCode(err - NWC24i_MSG_ERROR_CODE_BASE);
    }

    return err;
}

static NWC24Err CorrectHeadersForMBDelay(NWC24iMsgObj* pMsg) {
    NWC24UserId myId = 0;
    NWC24Err result;
    if (!NWC24i_MSGOBJ_GET_MB_DELAY(pMsg)) {
        return NWC24_OK;
    }
    if (!(pMsg->flags & NWC24_MSGOBJ_FOR_MENU)) {
        return NWC24_ERR_NOT_SUPPORTED;
    }
    result = NWC24GetMyUserId(&myId);
    if (result < 0) {
        return result;
    }
    if (pMsg->numTo != 1 || pMsg->to[0].id != myId) {
        return NWC24_ERR_INVALID_VALUE;
    }
    result = NWC24SetMsgDesignatedTime((NWC24MsgObj*)pMsg, ((pMsg->msgBoard >> 16) & 255) * 60);
    if (result < 0) {
        pMsg->command &= ~0x40000;
    }
    return NWC24_OK;
}

static NWC24Err CheckMsgObject(const NWC24iMsgObj* pMsg) {
    const char* pSubject;
    const char* pIt;

    if (pMsg->numTo <= 0) {
        return NWC24_ERR_NULL;
    }

    if (pMsg->numTo > NWC24i_MSG_RECIPIENT_MAX) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (pMsg->subject.pData != NULL) {
        for (pIt = (const char*)pMsg->subject.pData; *pIt != '\0'; pIt++) {
            if (pIt[0] == '\r' && pIt[1] != '\n') {
                return NWC24_ERR_FORMAT;
            }

            if (pIt[0] == '\n' && pIt[1] != ' ') {
                return NWC24_ERR_FORMAT;
            }
        }
    }

    if (pMsg->numAttached > NWC24i_MSG_ATTACHMENT_MAX) {
        return NWC24_ERR_INVALID_VALUE;
    }

    return NWC24_OK;
}

static NWC24Err CheckMsgBoxSpace(const NWC24iMsgObj* pMsg, NWC24MsgBoxId id) {
    NWC24Err result;
    u32 textSize;
    u32 totalSize;
    int i;

    totalSize = 0;

    for (i = 0; i < pMsg->numAttached; i++) {
        totalSize += (pMsg->attachedSize[i] * 4 + 2) / 3 + (pMsg->attachedSize[i] / B64_STEP * 2) + 4;
    }

    switch (pMsg->encoding) {
    case NWC24_ENC_7BIT:
    case NWC24_ENC_8BIT: {
        textSize = pMsg->text.size;
        break;
    }

    case NWC24_ENC_BASE64: {
        textSize = (pMsg->text.size * 4 + 2) / 3 + (pMsg->text.size / B64_STEP * 2) + 4;
        break;
    }

    case NWC24_ENC_QUOTED_PRINTABLE: {
        textSize = pMsg->text.size * 4 / 3;
        break;
    }

    default: {
        textSize = 0;
        break;
    }
    }

    totalSize += textSize;

    if (totalSize >= NWC24i_MBOX_MAX_SIZE) {
        return NWC24_ERR_OVERFLOW;
    }

    result = NWC24iMBoxCheck(id, totalSize + NWC24i_STRING_WORK_SIZE);
    return result != NWC24_OK ? result : NWC24_OK;
}

static NWC24Err SynthesizeAddrStr(const NWC24iAddr* pAddr, u32 flags, char* pBuffer, int bufferSize, u32* pWritten) {
    s32 written;
    char idBuffer[NWC24i_WII_ID_LEN + 1];
    NWC24Err result;
    const char* pDomain;

    result = NWC24_OK;

    if (flags & NWC24_MSGOBJ_FOR_RECIPIENT) {
        pDomain = NWC24GetAccountDomain();
        written = Mail_strlen(pDomain);

        if (written <= 0) {
            result = NWC24_ERR_CONFIG;
            goto _exit;
        }

        if (written + NWC24i_WII_ID_LEN + 2 >= bufferSize) {
            result = NWC24_ERR_NOMEM;
            goto _exit;
        }

        NWC24iConvIdToStr(pAddr->id, idBuffer);
        written = Mail_sprintf(pBuffer, "%c%s%s", 'w', idBuffer, pDomain);

        if (written == 0) {
            result = NWC24_ERR_FATAL;
            goto _exit;
        }

        *pWritten = written;

    } else if (flags & NWC24_MSGOBJ_FOR_PUBLIC) {
        if (pAddr->data.size + 3 >= bufferSize) {
            result = NWC24_ERR_NOMEM;
            goto _exit;
        }

        written = Mail_sprintf(pBuffer, "%s", pAddr->data.pData);

        if (written == 0) {
            result = NWC24_ERR_FATAL;
            goto _exit;
        }

        *pWritten = written;

    } else {
        result = NWC24_ERR_INVALID_VALUE;
    }

_exit:
    return result;
}

static NWC24Err WriteSMTP_MAILFROM(NWC24iMsgObj* pMsg) {
    char* pWork;
    u32 addrFlags;
    u32 addrLen;
    s32 totalLen;
    s32 remainSize;
    NWC24Err result;

    totalLen = 0;
    result = NWC24_OK;

    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

    Mail_strncat(pWork, "MAIL FROM: ", MSG_WORK_SIZE);
    pWork += NWC24i_KSTRLEN("MAIL FROM: ") - 1;

    addrFlags = NWC24_MSGOBJ_FOR_RECIPIENT;
    if (pMsg->flags & NWC24_MSGOBJ_FLAG_20) {
        addrFlags = NWC24_MSGOBJ_FOR_PUBLIC;
    }

    remainSize = MSG_WORK_SIZE - (NWC24i_KSTRLEN("MAIL FROM: ") - 1) - (NWC24i_KSTRLEN("\r\n") - 1);
    result = SynthesizeAddrStr(&pMsg->from, addrFlags, pWork, remainSize, &addrLen);
    if (result != NWC24_OK) {
        return result;
    }

    pWork += addrLen;
    *pWork++ = '\r';
    *pWork++ = '\n';

    remainSize -= addrLen;
    totalLen = addrLen + NWC24i_KSTRLEN("MAIL FROM: ") - 1 + NWC24i_KSTRLEN("\r\n") - 1;

    if (remainSize <= 0) {
        return NWC24_ERR_NOMEM;
    }

    if (result == NWC24_OK) {
        result = NWC24FWrite(NWC24WorkP->stringWork, totalLen, m_pFile);

        if (result == NWC24_OK) {
            pMsg->length += totalLen;
        }
    }

    return result;
}

static NWC24Err WriteSMTP_RCPTTO(NWC24iMsgObj* pMsg) {
    char* pWork;
    u32 i;
    u32 addrLen;
    u32 totalLen;
    s32 remainSize;
    NWC24Err result;

    totalLen = 0;
    result = NWC24_OK;

    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

    remainSize = MSG_WORK_SIZE;

    for (i = 0; i < pMsg->numTo; i++) {
        Mail_strncat(pWork, "RCPT TO: ", remainSize);
        pWork += NWC24i_KSTRLEN("RCPT TO: ") - 1;

        remainSize -= (NWC24i_KSTRLEN("RCPT TO: ") - 1) + (NWC24i_KSTRLEN("\r\n") - 1);
        result = SynthesizeAddrStr(&pMsg->to[i], pMsg->flags, pWork, remainSize, &addrLen);
        if (result != NWC24_OK) {
            break;
        }

        pWork += addrLen;
        *pWork++ = '\r';
        *pWork++ = '\n';

        remainSize -= addrLen;

        totalLen += addrLen + NWC24i_KSTRLEN("RCPT TO: ") - 1 + NWC24i_KSTRLEN("\r\n") - 1;

        if (remainSize <= 0) {
            result = NWC24_ERR_NOMEM;
            break;
        }
    }

    if (result == NWC24_OK) {
        result = NWC24FWrite(NWC24WorkP->stringWork, totalLen, m_pFile);

        if (result == NWC24_OK) {
            pMsg->length += totalLen;
        }
    }

    return result;
}

static NWC24Err WriteSMTP_DATA(NWC24iMsgObj* pMsg) {
    NWC24Err result;
    char* pWork;

    result = NWC24_OK;

    pWork = NWC24WorkP->stringWork;
    Mail_strcpy(pWork, "DATA\r\n\0");

    result = NWC24FWrite(pWork, Mail_strlen(pWork), m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += NWC24i_KSTRLEN("DATA\r\n") - 1;
    }

    return result;
}

static NWC24Err WriteFromField(NWC24iMsgObj* pMsg) {
    char* pWork;
    u32 addrFlags;
    u32 addrLen;
    s32 totalLen;
    s32 remainSize;
    NWC24Err result;

    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

    Mail_strncat(pWork, "From: ", MSG_WORK_SIZE);
    pWork += NWC24i_KSTRLEN("From: ") - 1;

    pMsg->fromField.offset = pMsg->length + NWC24i_KSTRLEN("From: ") - 1;

    addrFlags = NWC24_MSGOBJ_FOR_RECIPIENT;
    if (pMsg->flags & NWC24_MSGOBJ_FLAG_20) {
        addrFlags = NWC24_MSGOBJ_FOR_PUBLIC;
    }

    remainSize = MSG_WORK_SIZE - (NWC24i_KSTRLEN("From: ") - 1) - (NWC24i_KSTRLEN("\r\n") - 1);
    result = SynthesizeAddrStr(&pMsg->from, addrFlags, pWork, remainSize, &addrLen);
    if (result != NWC24_OK) {
        return result;
    }

    pWork += addrLen;
    *pWork++ = '\r';
    *pWork++ = '\n';

    remainSize -= addrLen;
    totalLen = addrLen + NWC24i_KSTRLEN("From: ") - 1 + NWC24i_KSTRLEN("\r\n") - 1;

    if (remainSize <= 0) {
        return NWC24_ERR_NOMEM;
    }

    if (result == NWC24_OK) {
        result = NWC24FWrite(NWC24WorkP->stringWork, totalLen, m_pFile);

        if (result == NWC24_OK) {
            pMsg->length += totalLen;

            pMsg->fromField.size = pMsg->length - pMsg->fromField.offset - (NWC24i_KSTRLEN("\r\n") - 1);
        }
    }

    return result;
}

static NWC24Err WriteToField(NWC24iMsgObj* pMsg) {
    char* pWork;
    s32 i;
    u32 len;
    u32 totalLen;
    s32 remainSize;
    NWC24Err result;

    result = NWC24_OK;

    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

    Mail_strncat(pWork, "To: ", MSG_WORK_SIZE);
    pWork += NWC24i_KSTRLEN("To: ") - 1;

    i = 0;
    totalLen = NWC24i_KSTRLEN("To: ") - 1;
    remainSize = MSG_WORK_SIZE - (NWC24i_KSTRLEN("To: ") - 1);

    pMsg->toField.offset = pMsg->length + NWC24i_KSTRLEN("To: ") - 1;

    for (; i < pMsg->numTo; i++) {
        result = SynthesizeAddrStr(&pMsg->to[i], pMsg->flags, pWork, remainSize, &len);
        if (result != NWC24_OK) {
            break;
        }

        remainSize -= len;
        pWork += len;
        totalLen += len;

        if (remainSize <= NWC24i_KSTRLEN("To: ") - 1) {
            result = NWC24_ERR_NOMEM;
            break;
        }

        if (i < pMsg->numTo - 1) {
            *pWork++ = ',';
            *pWork++ = '\r';
            *pWork++ = '\n';
            *pWork++ = ' ';

            totalLen += NWC24i_KSTRLEN("To: ") - 1;
            remainSize -= NWC24i_KSTRLEN("To: ") - 1;
        }
    }

    *pWork++ = '\r';
    *pWork++ = '\n';
    totalLen += NWC24i_KSTRLEN("\r\n") - 1;

    if (result == NWC24_OK) {
        result = NWC24FWrite(NWC24WorkP, totalLen, m_pFile);

        if (result == NWC24_OK) {
            pMsg->length += totalLen;

            pMsg->toField.size = pMsg->length - pMsg->toField.offset - (NWC24i_KSTRLEN("\r\n") - 1);
        }
    }

    return result;
}

static NWC24Err WriteSubjectField(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;

    pWork = NWC24WorkP->stringWork;
    result = NWC24_OK;

    if (pMsg->subject.size == 0) {
        return NWC24_ERR_NULL;
    }

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_strcpy(pWork, "Subject: ");
    Mail_strncat(pWork, (const char*)pMsg->subject.pData, pMsg->subject.size);
    Mail_strncat(pWork, "\r\n\0", sizeof("\r\n"));

    fieldLen = STD_strnlen(pWork, NWC24i_STRING_WORK_SIZE);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static char* MonthStr[NWC24i_MONTH_MAX] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

static NWC24Err WriteDateField(NWC24iMsgObj* pMsg) {
    char* pWork;
    u32 len;
    NWC24Err result;
    NWC24Date date;
    OSCalendarTime cal;

    NWC24Date_Init(&date);
    NETGetUniversalCalendar(&cal);

    date.year = cal.year;
    date.month = cal.mon + NWC24i_MONTH_MIN;
    date.day = cal.mday;
    date.hour = cal.hour;
    date.min = cal.min;
    date.sec = cal.sec;

    if (date.month > NWC24i_MONTH_MAX) {
        return NWC24_ERR_FORMAT;
    }

    NWC24iDateToMinutes(&pMsg->createTime, &date);
    if (pMsg->flags & (1 << 24)) {
        NWC24iMinutesToDate(&date, pMsg->unk2C);
    }

    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

    len = Mail_sprintf(pWork, "Date: %02d %s %d %02d:%02d:%02d -0000\r\n", date.day, MonthStr[date.month - NWC24i_MONTH_MIN], date.year, date.hour,
                       date.min, date.sec);

    len = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, len, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += len;
    }

    return result;
}

static NWC24Err WriteMessageIdField(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;
    char buffer[NWC24i_MSG_ID_LEN + 1 + 1];
    const char* pDomain;
    NWC24UserId myId;

    pWork = NWC24WorkP->stringWork;

    pDomain = NWC24GetAccountDomain();
    NWC24GetMyUserId(&myId);

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_strcpy(pWork, "Message-Id: <");

    Mail_memset(buffer, 0, sizeof(buffer));
    Mail_sprintf(buffer, "%05X%08X%08X%08X", pMsg->id, (u32)(myId >> 32), (u32)(myId >> 0), pMsg->createTime);
    Mail_strcat(pWork, buffer);

    Mail_strcat(pWork, pDomain);

    Mail_strcat(pWork, ">\r\n");

    fieldLen = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteXWiiAppIdField(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;
    char buffer[NWC24i_APP_ID_LEN + 1];
    u32 idFlag;

    idFlag = 0;
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_strcpy(pWork, "X-Wii-AppId: ");

    if (!NWC24IsMsgLibOpenedByTool() && NWC24GetAppId() != NWC24i_APP_ID_IPL && pMsg->appId != 0x48414341) {
        pMsg->appId = NWC24GetAppId();
    }

    if (pMsg->flags & NWC24_MSGOBJ_FOR_APP) {
        idFlag |= 1;
    }

    if (pMsg->flags & NWC24_MSGOBJ_FOR_MENU) {
        idFlag |= 2;
    }

    Mail_memset(buffer, 0, NWC24i_APP_ID_LEN + 1);
    Mail_sprintf(buffer, "%d-%08X-%04X\r\n", idFlag, pMsg->appId, pMsg->groupId);

    Mail_strcat(pWork, buffer);
    fieldLen = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteXWiiTagField(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;

    if (pMsg->tag == 0) {
        return NWC24_OK;
    }

    result = NWC24_OK;
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_sprintf(pWork, "X-Wii-Tag: %08X\r\n", pMsg->tag);

    fieldLen = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteXWiiCmdField(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;

    if (pMsg->command == 0) {
        return NWC24_OK;
    }

    result = NWC24_OK;
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_sprintf(pWork, "X-Wii-Cmd: %08X\r\n", pMsg->command);

    fieldLen = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteXWiiDWCIdField(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;

    if (!(pMsg->flags & NWC24_MSGOBJ_DWC)) {
        return NWC24_OK;
    }

    result = NWC24_OK;
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_sprintf(pWork, "X-Wii-DWCId: %08X\r\n", pMsg->dwcId.size);

    fieldLen = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteXWiiFaceField(NWC24iMsgObj* pMsg) {
    char* pWork;
    const u8* pDataPtr;
    u32 pos;
    NWC24Err result;
    s32 dataRemain;
    u32 encSize;
    s32 decSize;

    pos = 0;
    result = NWC24_OK;

    if (pMsg->face.size == 0) {
        return NWC24_OK;
    }

    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

    Mail_strcpy(pWork, "X-WiiFace:");
    pos = NWC24i_KSTRLEN("X-WiiFace:") - 1;

    pDataPtr = (const u8*)pMsg->face.pData;
    dataRemain = pMsg->face.size;

    while (dataRemain > 0) {
        pWork[pos++] = ' ';

        decSize = dataRemain;
        if (dataRemain > B64_DECODE_SIZE(B64_STEP)) {
            decSize = B64_DECODE_SIZE(B64_STEP);
        }

        result = NWC24Base64Encode(pDataPtr, decSize, pWork + pos, B64_ENCODE_SIZE(B64_STEP), &encSize);

        pDataPtr += decSize;
        dataRemain -= decSize;

        pos += encSize;

        pWork[pos++] = '\r';
        pWork[pos++] = '\n';
    }

    result = NWC24FWrite(pWork, pos, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += pos;
    }

    return result;
}

static NWC24Err WriteXWiiAltNameField(NWC24iMsgObj* pMsg) {
    char* pWork;
    const u8* pDataPtr;
    u32 pos;
    NWC24Err result;
    s32 dataRemain;
    u32 encSize;
    s32 decSize;

    pos = 0;
    result = NWC24_OK;

    if (pMsg->alt.size == 0) {
        return NWC24_OK;
    }

    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

    Mail_strcpy(pWork, "X-Wii-AltName:");
    pos = NWC24i_KSTRLEN("X-Wii-AltName:") - 1;

    pDataPtr = (const u8*)pMsg->alt.pData;
    dataRemain = pMsg->alt.size;

    while (dataRemain > 0) {
        pWork[pos++] = ' ';

        decSize = dataRemain;
        if (dataRemain > B64_DECODE_SIZE(B64_STEP)) {
            decSize = B64_DECODE_SIZE(B64_STEP);
        }

        result = NWC24Base64Encode(pDataPtr, decSize, pWork + pos, B64_ENCODE_SIZE(B64_STEP), &encSize);

        pDataPtr += decSize;
        dataRemain -= decSize;

        pos += encSize;

        pWork[pos++] = '\r';
        pWork[pos++] = '\n';
    }

    result = NWC24FWrite(pWork, pos, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += pos;
    }

    return result;
}

static NWC24Err WriteXWiiMBNoReplyField(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    BOOL noreply;

    noreply = NWC24i_MSGOBJ_GET_MB_NOREPLY(pMsg);
    if (!noreply) {
        return NWC24_OK;
    }

    result = NWC24_OK;
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_strcpy(pWork, "X-Wii-MB-NoReply: -\r\n");

    result = NWC24FWrite(pWork, NWC24i_KSTRLEN("X-Wii-MB-NoReply: -\r\n") - 1, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += NWC24i_KSTRLEN("X-Wii-MB-NoReply: -\r\n") - 1;
    }

    return result;
}

static NWC24Err WriteXWiiMBRegDateField(NWC24iMsgObj* pMsg) {
    NWC24Err result = NWC24_OK;
    char* pWork;
    s32 fieldLen;
    u32 regDate;

    if (NWC24i_MSGOBJ_GET_MB_REGDATE(pMsg) == 0) {
        return result;
    }

    regDate = NWC24i_MSGOBJ_GET_MB_REGDATE(pMsg);
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_sprintf(pWork, "X-Wii-MB-RegDate: %04X\r\n", regDate);

    fieldLen = STD_strnlen(pWork, NWC24i_STRING_WORK_SIZE);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteXWiiMBDelayField(NWC24iMsgObj* pMsg) {
    NWC24Err result = NWC24_OK;
    char* pWork;
    s32 fieldLen;
    u32 delay;

    if (NWC24i_MSGOBJ_GET_MB_DELAY(pMsg) == 0) {
        return result;
    }

    delay = NWC24i_MSGOBJ_GET_MB_DELAY(pMsg);
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_sprintf(pWork, "X-Wii-MB-Delay: %02X\r\n", delay >> 16);

    fieldLen = STD_strnlen(pWork, NWC24i_STRING_WORK_SIZE);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static char* ContentTypeA = "Content-Type: %s;\r\n name=%c%07d.%s\r\n";

static char* ContentTxEncA = "Content-Transfer-Encoding: base64\r\n";

static char* ContentDispA = "Content-Disposition: attachment;\r\n filename=%c%07d.%s\r\n\r\n";

static char* ContentTypeTP = "Content-Type: text/plain; charset=%s\r\n";

static char* ContentTxEncT = "Content-Transfer-Encoding: %s\r\n\r\n";

static NWC24Err WriteMIMETopHeader(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;

    result = NWC24_OK;
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_strcpy(pWork, "MIME-Version: 1.0\r\n");

    if (pMsg->flags & NWC24_MSGOBJ_MULTIPART) {
        Mail_strcat(pWork, "Content-Type: multipart/mixed;\r\n boundary=\"");
        Mail_strcat(pWork, MultiPartDivider);
        Mail_strcat(pWork, "\"\r\n");
    }

    fieldLen = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteMIMEPartBoundary(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;

    result = NWC24_OK;
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_sprintf(pWork, "\r\n--%s", MultiPartDivider);
    Mail_strcat(pWork, "\r\n");

    fieldLen = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteMIMEAttachHeader(NWC24iMsgObj* pMsg, s32 idx) {
    char* pWork;
    const char* pTypeStr;
    const char* pTypeSuffix;
    s32 len;
    s32 written;
    NWC24Err result;

    result = NWC24_OK;
    written = 0;

    Mail_memset(NWC24WorkP->stringWork, 0, NWC24i_STRING_WORK_SIZE);
    pWork = NWC24WorkP->stringWork;

    pTypeStr = NWC24GetMIMETypeStr(pMsg->attachedType[idx]);
    pTypeSuffix = NWC24iGetMIMETypeSuffix(pMsg->attachedType[idx]);

    len = Mail_strlen(ContentTypeA) + Mail_strlen(ContentTxEncA) + Mail_strlen(ContentDispA) + Mail_strlen(pTypeStr) + 4;

    if (len >= NWC24i_STRING_WORK_SIZE) {
        return NWC24_ERR_NOMEM;
    }

    len = Mail_sprintf(pWork, ContentTypeA, pTypeStr, (char)('a' + idx), pMsg->id, pTypeSuffix);
    if (len <= 0) {
        return NWC24_ERR_FAILED;
    }

    pWork += len;
    written += len;

    len = Mail_sprintf(pWork, ContentTxEncA);
    if (len <= 0) {
        return NWC24_ERR_FAILED;
    }

    pWork += len;
    written += len;

    len = Mail_sprintf(pWork, ContentDispA, (char)('a' + idx), pMsg->id, pTypeSuffix);
    if (len <= 0) {
        return NWC24_ERR_FAILED;
    }

    pWork += len;
    written += len;

    result = NWC24FWrite(NWC24WorkP->stringWork, written, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += written;
    }

    return result;
}

static NWC24Err WriteMIMEPartBoundaryEnd(NWC24iMsgObj* pMsg) {
    char* pWork;
    NWC24Err result;
    u32 fieldLen;

    result = NWC24_OK;
    pWork = NWC24WorkP->stringWork;

    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_sprintf(pWork, "\r\n--%s", MultiPartDivider);
    Mail_strcat(pWork, "--\r\n");

    fieldLen = Mail_strlen(pWork);

    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }

    return result;
}

static NWC24Err WriteContentTypeField(NWC24iMsgObj* pMsg) {
    char* pWork;
    const char* pCharset;
    const char* pEncoding;
    u32 total;
    NWC24Err result;
    int len;

    total = 0;
    result = NWC24_OK;

    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

    pCharset = NWC24GetCharsetStr(pMsg->charset);

    if (pCharset != NULL) {
        Mail_sprintf(pWork, ContentTypeTP, pCharset);
        pMsg->contentType.offset = pMsg->length + NWC24i_KSTRLEN("Content-Type: text/plain; charset=") - 1;

        len = Mail_strlen(pCharset);
        pMsg->contentType.size = len + 1;

        len = Mail_strlen(pWork);
        total += len;
        pWork += len;
    }

    pEncoding = NWC24GetEncodingStr(pMsg->encoding);

    if (pEncoding != NULL) {
        Mail_sprintf(pWork, ContentTxEncT, pEncoding);
        pMsg->txEncoding.offset = pMsg->length + total + NWC24i_KSTRLEN("Content-Transfer-Encoding: ") - 1;

        len = Mail_strlen(pEncoding);
        pMsg->txEncoding.size = len + 1;

        len = Mail_strlen(pWork);
        total += len;
        pWork += len;
    }

    if (total == 0) {
        return NWC24_OK;
    }

    result = NWC24FWrite(NWC24WorkP->stringWork, total, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += total;
    }

    return result;
}

static NWC24Err WriteTextBody(NWC24iMsgObj* pMsg) {
    NWC24Err result;

    result = WritePlainText(pMsg);
    if (result == NWC24_ERR_NULL) {
        return NWC24_OK;
    }

    return result;
}

static NWC24Err WriteUserField(NWC24iMsgObj* pMsg) {
    NWC24Err result;

    result = NWC24_OK;

    if (pMsg->user.size == 0) {
        return NWC24_OK;
    }

    result = NWC24FWrite(pMsg->user.pData, pMsg->user.size, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += pMsg->user.size;
    }

    return result;
}

static NWC24Err WritePlainText(NWC24iMsgObj* pMsg) {
    NWC24Err result;
    s32 bytesWritten;

    result = NWC24_OK;

    if (pMsg->text.size == 0) {
        return NWC24_ERR_NULL;
    }

    switch (pMsg->encoding) {
    case NWC24_ENC_7BIT:
    case NWC24_ENC_8BIT: {
        result = NWC24FWrite(pMsg->text.pData, pMsg->text.size, m_pFile);
        bytesWritten = pMsg->text.size;
        break;
    }

    case NWC24_ENC_BASE64: {
        result = WriteBase64Data(pMsg->text.pData, pMsg->text.size, &bytesWritten);
        break;
    }

    case NWC24_ENC_QUOTED_PRINTABLE: {
        result = WriteQPData(pMsg->text.pData, pMsg->text.size, &bytesWritten);
        break;
    }

    default: {
        result = NWC24_ERR_INVALID_VALUE;
        break;
    }
    }

    if (result == NWC24_OK) {
        pMsg->dwcId.offset = pMsg->text.size;
        pMsg->length += bytesWritten;
    }

    return result;
}

static NWC24Err WriteAttachData(NWC24iMsgObj* pMsg, s32 idx) {
    NWC24Err result;
    s32 written;

    result = WriteBase64Data(pMsg->attached[idx].pData, pMsg->attached[idx].size, &written);

    if (result == NWC24_OK) {
        pMsg->length += written;
    }

    return result;
}

static NWC24Err WriteBase64Data(const void* pData, s32 size, s32* pWritten) {
    char* pWork;
    const u8* pDataPtr;
    s32 dataRemain;
    s32 totalWritten;
    NWC24Err result;
    u32 encSize;

    pDataPtr = (const u8*)pData;
    dataRemain = size;

    pWork = NWC24WorkP->stringWork;
    totalWritten = 0;
    result = NWC24_OK;

    while (dataRemain >= B64_STEP) {
        Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

        result = NWC24Base64Encode(pDataPtr, B64_STEP, pWork, B64_ENCODE_SIZE(B64_STEP), &encSize);
        if (result != NWC24_OK) {
            break;
        }

        pWork[encSize++] = '\r';
        pWork[encSize++] = '\n';
        pWork[encSize] = '\0';

        result = NWC24FWrite(pWork, encSize, m_pFile);
        if (result != NWC24_OK) {
            break;
        }

        pDataPtr += B64_STEP;
        dataRemain -= B64_STEP;

        totalWritten += encSize;
    }

    if (result == NWC24_OK) {
        Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

        result = NWC24Base64Encode(pDataPtr, dataRemain, pWork, B64_ENCODE_SIZE(B64_STEP), &encSize);

        if (result == NWC24_OK) {
            pWork[encSize++] = '\r';
            pWork[encSize++] = '\n';
            pWork[encSize++] = '\r';
            pWork[encSize++] = '\n';
            pWork[encSize] = '\0';

            result = NWC24FWrite(pWork, encSize, m_pFile);
            if (result == NWC24_OK) {
                totalWritten += encSize;
            }
        }
    }

    *pWritten = totalWritten;
    return result;
}

static NWC24Err WriteQPData(const void* pData, s32 size, s32* pWritten) {
    char* pWork;
    const u8* pDataPtr;
    s32 dataRemain;
    s32 totalWritten;
    NWC24Err result;

    pDataPtr = (const u8*)pData;
    dataRemain = size;

    pWork = NWC24WorkP->stringWork;
    totalWritten = 0;
    result = NWC24_OK;

    while (dataRemain > 0) {
        u32 dataRead;
        u32 encSize;

        if (totalWritten > 0) {
            Mail_memcpy(pWork, "=\r\n", NWC24i_KSTRLEN("=\r\n") - 1);

            result = NWC24FWrite(pWork, NWC24i_KSTRLEN("=\r\n") - 1, m_pFile);
            if (result != NWC24_OK) {
                break;
            }

            totalWritten += NWC24i_KSTRLEN("=\r\n") - 1;
        }

        Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);

        result = NWC24EncodeQuotedPrintable(pWork, NWC24i_STRING_WORK_SIZE, &encSize, pDataPtr, dataRemain, &dataRead);

        if (result != NWC24_OK && result != NWC24_ERR_OVERFLOW) {
            break;
        }

        result = NWC24FWrite(pWork, encSize, m_pFile);
        if (result != NWC24_OK) {
            break;
        }

        pDataPtr += dataRead;
        dataRemain -= dataRead;

        totalWritten += encSize;
    }

    *pWritten = totalWritten;
    return result;
}

static NWC24Err WriteXWiiIconNewField(NWC24iMsgObj* pMsg) {
    char* pWork;
    u32 fieldLen;
    NWC24Err result;
    if (pMsg->iconNew == 0 || pMsg->iconNew == 0x80000000) {
        return NWC24_OK;
    }
    pWork = NWC24WorkP->stringWork;
    Mail_memset(pWork, 0, NWC24i_STRING_WORK_SIZE);
    Mail_sprintf(pWork, "X-Wii-IconNew: %08X\r\n", pMsg->iconNew);
    fieldLen = Mail_strlen(pWork);
    result = NWC24FWrite(pWork, fieldLen, m_pFile);
    if (result == NWC24_OK) {
        pMsg->length += fieldLen;
    }
    return result;
}
