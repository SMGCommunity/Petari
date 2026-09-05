#include "revolution/nwc24.h"
#include "revolution/nwc24/NWC24Config.h"
#include "revolution/nwc24/NWC24FileApi.h"
#include "revolution/nwc24/NWC24Manage.h"
#include "revolution/nwc24/NWC24MsgCommit.h"
#include "revolution/nwc24/NWC24StdApi.h"
#include "revolution/nwc24/NWC24Utils.h"

typedef union NWC24AddrOrId {
    u64 id;
    NWC24Data addr;
} NWC24AddrOrId;

int LoopBackEnable = 1;
NWC24File* m_pFile;

NWC24Err NWC24CommitMsgInternal(NWC24MsgObj* obj, NWC24MsgBoxId mBoxId);

NWC24Err SynthesizeAddrStr(const NWC24AddrOrId* addr, u32 type, char* dst, s32 maxLen, u32* lenOut);
NWC24Err WriteSMTP_MAILFROM(NWC24MsgObj* obj);
NWC24Err WriteSMTP_RCPTTO(NWC24MsgObj* obj);
NWC24Err WriteFromField(NWC24MsgObj* obj);
NWC24Err WriteToField(NWC24MsgObj* obj);

NWC24Err NWC24CommitMsg(NWC24MsgObj* obj) {
    NWC24MsgBoxId mBoxId;
    u64 myId;

    mBoxId = NWC24_SEND_BOX;

    if (!NWC24IsMsgLibOpened() && !NWC24IsMsgLibOpenedByTool()) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }

    if (!(obj->flags & 0x100) || (obj->flags & 0x200)) {
        return NWC24_ERR_PROTECTED;
    }

    if (LoopBackEnable && (obj->flags & 1)) {
        NWC24GetMyUserId(&myId);
        if (obj->numTo == 1 && obj->toIds[0] == myId) {
            mBoxId = NWC24_RECV_BOX;
        }
    }

    return NWC24CommitMsgInternal(obj, mBoxId);
}

NWC24Err CheckMsgObject(const NWC24MsgObj* obj) {
    const char* ptr;

    if (obj->numTo == 0) {
        return NWC24_ERR_NULL;
    }

    if (obj->numTo > 8) {
        return NWC24_ERR_INVALID_VALUE;
    }

    ptr = obj->subject.ptr;
    if (ptr != NULL) {
        for (; *ptr != '\0'; ptr++) {
            if (*ptr == '\r') {
                if (ptr[1] != '\n') {
                    return NWC24_ERR_FORMAT;
                }
            }
            if (*ptr == '\n') {
                if (ptr[1] != ' ') {
                    return NWC24_ERR_FORMAT;
                }
            }
        }
    }

    if (obj->numAttached > 2) {
        return NWC24_ERR_INVALID_VALUE;
    }

    return NWC24_OK;
}

NWC24Err SynthesizeAddrStr(const NWC24AddrOrId* addr, u32 type, char* dst, s32 maxLen, u32* lenOut) {
    NWC24Err err = NWC24_OK;
    s32 len;

    if (type & 1) {
        char idStr[NWC24i_WII_ID_LEN + 1];
        char* domain = NWC24GetAccountDomain();
        s32 domainLen = Mail_strlen(domain);

        if (domainLen <= 0) {
            err = NWC24_ERR_CONFIG;
        } else if (domainLen + 0x12 >= maxLen) {
            err = NWC24_ERR_NOMEM;
        } else {
            NWC24iConvIdToStr(addr->id, idStr);
            len = Mail_sprintf(dst, "%c%s%s", 'w', idStr, domain);
            if (len == 0) {
                err = NWC24_ERR_FATAL;
            } else {
                *lenOut = len;
            }
        }
    } else if (type & 2) {
        if (addr->addr.size + 3 >= maxLen) {
            err = NWC24_ERR_NOMEM;
        } else {
            len = Mail_sprintf(dst, "%s", addr->addr.ptr);
            if (len == 0) {
                err = NWC24_ERR_FATAL;
            } else {
                *lenOut = len;
            }
        }
    } else {
        err = NWC24_ERR_INVALID_VALUE;
    }

    return err;
}

NWC24Err WriteSMTP_MAILFROM(NWC24MsgObj* obj) {
    char* ptr = NWC24WorkP->stringWork;
    u32 synthType;
    u32 len;
    u32 total;
    NWC24Err err;

    Mail_memset(ptr, 0, sizeof(NWC24WorkP->stringWork));
    Mail_strncat(ptr, "MAIL FROM: ", 0x3fe);
    ptr += 0xb;

    if (obj->flags & 0x100000) {
        synthType = 2;
    } else {
        synthType = 1;
    }

    err = SynthesizeAddrStr((const NWC24AddrOrId*)&obj->fromId, synthType, ptr, 0x3f1, &len);
    if (err == NWC24_OK) {
        ptr += len;
        *ptr = '\r';
        ptr[1] = '\n';

        if ((s32)(0x3f1 - len) <= 0) {
            return NWC24_ERR_NOMEM;
        }

        total = len + 0xd;
        if (err == NWC24_OK) {
            err = NWC24FWrite(NWC24WorkP->stringWork, total, m_pFile);
            if (err == NWC24_OK) {
                obj->length += total;
            }
        }
    }

    return err;
}

NWC24Err WriteSMTP_RCPTTO(NWC24MsgObj* obj) {
    char* ptr;
    u32 i;
    s32 count;
    s32 remainSize;
    u32 len;
    NWC24Err err;

    ptr = NWC24WorkP->stringWork;
    count = 0;
    err = NWC24_OK;

    Mail_memset(ptr, 0, sizeof(NWC24WorkP->stringWork));

    remainSize = 0x3fe;
    for (i = 0; i < obj->numTo; i++) {
        Mail_strncat(ptr, "RCPT TO: ", remainSize);
        ptr += 9;
        remainSize -= 0xb;

        err = SynthesizeAddrStr((const NWC24AddrOrId*)&obj->toIds[i], obj->flags, ptr, remainSize, &len);
        if (err != NWC24_OK) {
            break;
        }

        ptr += len;
        *ptr = '\r';
        ptr[1] = '\n';
        ptr += 2;

        remainSize -= len;
        count += len + 0xb;
        if (remainSize <= 0) {
            err = NWC24_ERR_NOMEM;
            break;
        }
    }

    if (err == NWC24_OK) {
        err = NWC24FWrite(NWC24WorkP->stringWork, count, m_pFile);
        if (err == NWC24_OK) {
            obj->length += count;
        }
    }

    return err;
}

NWC24Err WriteFromField(NWC24MsgObj* obj) {
    char* ptr = NWC24WorkP->stringWork;
    u32 synthType;
    u32 len;
    u32 total;
    NWC24Err err;

    Mail_memset(ptr, 0, sizeof(NWC24WorkP->stringWork));
    Mail_strncat(ptr, "From: ", 0x3fe);
    ptr += 0x6;

    obj->DATA_0x30.ptr = (const void*)(obj->length + 6);

    if (obj->flags & 0x100000) {
        synthType = 2;
    } else {
        synthType = 1;
    }

    err = SynthesizeAddrStr((const NWC24AddrOrId*)&obj->fromId, synthType, ptr, 0x3f6, &len);
    if (err == NWC24_OK) {
        ptr += len;
        *ptr = '\r';
        ptr[1] = '\n';

        total = len + 8;

        if ((s32)(0x3f6 - len) <= 0) {
            return NWC24_ERR_NOMEM;
        }

        if (err == NWC24_OK) {
            err = NWC24FWrite(NWC24WorkP->stringWork, total, m_pFile);
            if (err == NWC24_OK) {
                obj->length += total;
                obj->DATA_0x30.size = len;
            }
        }
    }

    return err;
}

NWC24Err WriteToField(NWC24MsgObj* obj) {
    char* ptr = NWC24WorkP->stringWork;
    u32 i;
    s32 remainSize;
    s32 total;
    NWC24Err err;
    u32 len;

    Mail_memset(ptr, 0, sizeof(NWC24WorkP->stringWork));
    Mail_strncat(ptr, "To: ", 0x3fe);
    ptr += 4;

    obj->DATA_0x38.ptr = (const void*)(obj->length + 4);

    total = 4;
    remainSize = 0x3fa;

    for (i = 0; i < obj->numTo; i++) {
        err = SynthesizeAddrStr((const NWC24AddrOrId*)&obj->toIds[i], obj->flags, ptr, remainSize, &len);
        if (err != NWC24_OK) {
            break;
        }

        remainSize -= len;
        ptr += len;
        total += len;

        if (remainSize <= 4) {
            err = NWC24_ERR_NOMEM;
            break;
        }

        if (i < (u32)(obj->numTo - 1)) {
            *ptr = ',';
            total += 4;
            remainSize -= 4;
            ptr[1] = '\r';
            ptr[2] = '\n';
            ptr[3] = ' ';
            ptr += 4;
        }
    }

    *ptr = '\r';
    ptr[1] = '\n';

    if (err == NWC24_OK) {
        err = NWC24FWrite(NWC24WorkP->stringWork, total + 2, m_pFile);
        if (err == NWC24_OK) {
            obj->length += total + 2;
            obj->DATA_0x38.size = (u32)obj->length - (u32)obj->DATA_0x38.ptr - 2;
        }
    }

    return err;
}
