#include <RVLFaceLibInternal.h>
#include <revolution/wpad.h>
#include <revolution/mem/expHeap.h>

#define MAGIC_CONTROLLER_DATA 'RNCD'
#define DB_REMOTE_MEM_ADDR 0
#define READ_RETRY_MAX 3

static void readcallback_(s32 chan, s32 result);

static void clearDatabase_(RFLiCtrlBuf* buf) {
    int i;

    buf->identifier = MAGIC_CONTROLLER_DATA;
    buf->secretFlag = 0;
    buf->deleted = 0;
    buf->checksum = 0;

    for (i = 0; i < RFL_CTRL_CHAR_MAX; i++) {
        memset(&buf->data[i], 0, sizeof(RFLiCharData));
    }
}

void RFLiInitCtrlBuf(MEMiHeapHead* heap) {
    RFLiCtrlBufManager* mgr;
    void* buffer;
    int i;

    mgr = RFLiGetCtrlBufManager();

    for (i = 0; i < 4; i++) {
        mgr->buffer[i] = MEMAllocFromExpHeapEx(heap, sizeof(RFLiCtrlBuf), 32);
        clearDatabase_(mgr->buffer[i]);
        mgr->loaded[i] = FALSE;
    }

    mgr->replaceBuf[0] = MEMAllocFromExpHeapEx(heap, sizeof(RFLiCtrlBuf), 32);
    clearDatabase_(mgr->replaceBuf[0]);

    mgr->replaceBuf[1] = MEMAllocFromExpHeapEx(heap, sizeof(RFLiCtrlBuf), 32);
    clearDatabase_(mgr->replaceBuf[1]);

    mgr->readBuffer = NULL;
    mgr->writeBuffer = NULL;
    mgr->writeChan = -1;
    mgr->read = TRUE;
    mgr->verifyBuffer = NULL;

    buffer = MEMAllocFromExpHeapEx(
        heap, RFLGetMiddleDBBufferSize(RFL_CTRL_CHAR_MAX), 32);

    RFLInitMiddleDB(&mgr->hiddenMDB, RFLMiddleDBType_HiddenRandom, buffer,
                    RFL_CTRL_CHAR_MAX);
}

static BOOL RFLiCheckCtrlBufferCore(const RFLiCtrlBuf* buf, u8 index,
                             RFLiHiddenType type) {
    u16 mask;

    mask = 1 << index;

    if (type == RFLiHiddenType_No) {
        if (buf->secretFlag & mask) {
            return FALSE;
        }
    } else if (type == RFLiHiddenType_Yes) {
        if (!(buf->secretFlag & mask)) {
            return FALSE;
        }
    }

    return RFLiIsValidID(&buf->data[index].createID);
}

static void validBufferFound_(RFLiCtrlBuf* buf, s32 chan) {
    RFLiCtrlBufManager* mgr;
    RFLiCharInfo info;
    int i;

    mgr = RFLiGetCtrlBufManager();
    memcpy(mgr->buffer[chan], buf, sizeof(RFLiCtrlBuf));

    for (i = 0; i < RFL_CTRL_CHAR_MAX; i++) {
        if (RFLiIsValidID(&mgr->buffer[chan]->data[i].createID)) {
            RFLiConvertRaw2Info(&mgr->buffer[chan]->data[i], &info);

            if (!RFLiCheckValidInfo(&info) || !RFLiIsValidOnNAND(&info)) {
                memset(&mgr->buffer[chan]->data[i], 0, sizeof(RFLiCharData));
            }
        }
    }

    mgr->loaded[chan] = TRUE;

    RFLiFree(mgr->tempBuffer);
    mgr->tempBuffer = NULL;

    if (RFLiGetIsolation()) {
        RFLiEndWorking(RFLErrcode_Success);
    } else {
        RFLiWriteCtrlToHiddenDB(mgr->buffer[chan],
                                RFLiGetCtrlBufManager()->readIsChMode);
    }
}

static BOOL checkCRC_(const RFLiCtrlBuf* buf) {
    if (buf->identifier == MAGIC_CONTROLLER_DATA) {
        return RFLiCalculateCRC(buf, sizeof(RFLiCtrlBuf)) == 0;
    }

    return FALSE;
}

static BOOL errorAndReRead_(s32 chan) {
    RFLiCtrlBufManager* mgr;
    BOOL retried;
    s32 reason;

    mgr = RFLiGetCtrlBufManager();
    retried = FALSE;

    if (++mgr->retry < READ_RETRY_MAX) {
        reason =
            WPADReadFaceData(chan, mgr->tempBuffer,
                             sizeof(RFLiCtrlBuf) * RFLi_CTRL_REPLACE_BUF_NUM,
                             DB_REMOTE_MEM_ADDR, readcallback_);
        if (reason != WPAD_RESULT_SUCCESS) {
            RFLiEndWorkingReason(RFLErrcode_Controllerfail, reason);
        }

        retried = TRUE;
    }

    return retried;
}

static void clearDeleted_(RFLiCtrlBuf* buf) {
    int i;
    u16 mask;

    for (i = 0; i < RFL_CTRL_CHAR_MAX; i++) {
        mask = 1 << i;
        if (buf->deleted & mask) {
            memset(&buf->data[i], 0, sizeof(RFLiCharData));
        }
    }
}

static void readcallback_(s32 chan, s32 result) {
    RFLiCtrlBufManager* mgr;
    RFLiCtrlBuf* buf;
    RFLiCtrlBuf* validBuf;

    mgr = RFLiGetCtrlBufManager();

    switch (result) {
    case WPAD_RESULT_SUCCESS:
        validBuf = NULL;
        buf = mgr->tempBuffer;

        // First block
        if (checkCRC_(buf)) {
            clearDeleted_(buf);
            memcpy(mgr->replaceBuf[0], buf, sizeof(RFLiCtrlBuf));

            if (validBuf == NULL) {
                validBuf = buf;
            }
        }

        buf++;

        // Second block
        if (checkCRC_(buf)) {
            clearDeleted_(buf);
            memcpy(mgr->replaceBuf[1], buf, sizeof(RFLiCtrlBuf));

            if (validBuf == NULL) {
                validBuf = buf;
            }
        }

        if (validBuf != NULL) {
            validBufferFound_(validBuf, chan);
        } else if (!errorAndReRead_(chan)) {
            RFLiFree(mgr->tempBuffer);
            mgr->tempBuffer = NULL;

            RFLiEndWorking(RFLErrcode_Loadfail);
            mgr->loaded[chan] = FALSE;
        }
        break;
    case WPAD_RESULT_ERR_3:
        if (!errorAndReRead_(chan)) {
            RFLiFree(mgr->tempBuffer);
            mgr->tempBuffer = NULL;

            RFLiEndWorkingReason(RFLErrcode_Controllerfail, WPAD_RESULT_ERR_3);
            mgr->loaded[chan] = FALSE;
        }
        break;
    default:
        RFLiFree(mgr->tempBuffer);
        mgr->tempBuffer = NULL;
        RFLiEndWorkingReason(RFLErrcode_Controllerfail, WPAD_RESULT_ERR_1);
        break;
    }
}

static void readbuffer_(s32 chan, RFLiCtrlBuf* dst, BOOL ch) {
    RFLiCtrlBufManager* mgr;
    RFLiCtrlBuf* buf;
    s32 reason;
    u32 type;

    mgr = RFLiGetCtrlBufManager();

    reason = WPADProbe(chan, &type);
    if (reason != WPAD_RESULT_SUCCESS) {
        RFLiEndWorkingReason(RFLErrcode_Controllerfail, reason);
        return;
    }

    buf = RFLiAlloc32(sizeof(RFLiCtrlBuf) * RFLi_CTRL_REPLACE_BUF_NUM);
    mgr->tempBuffer = buf;
    mgr->readBuffer = dst;
    mgr->readIsChMode = ch;
    mgr->retry = FALSE;

    reason = WPADReadFaceData(chan, buf,
                              sizeof(RFLiCtrlBuf) * RFLi_CTRL_REPLACE_BUF_NUM,
                              DB_REMOTE_MEM_ADDR, readcallback_);
    if (reason != WPAD_RESULT_SUCCESS) {
        RFLiFree(mgr->tempBuffer);
        mgr->tempBuffer = NULL;
        RFLiEndWorkingReason(RFLErrcode_Controllerfail, reason);
    }
}

static RFLErrcode RFLiLoadControllerAsync(s32 chan, BOOL ch) {
    RFLiCtrlBufManager* mgr;

    if (chan < 0 || chan >= 4) {
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    if (RFLiIsWorking()) {
        return RFLErrcode_NotAvailable;
    }

    mgr = RFLiGetCtrlBufManager();
    if (mgr == NULL) {
        return RFLErrcode_NotAvailable;
    }

    RFLiStartWorking();
    memset(mgr->buffer[chan], 0, sizeof(RFLiCtrlBuf));
    memset(mgr->replaceBuf[0], 0, sizeof(RFLiCtrlBuf));
    memset(mgr->replaceBuf[1], 0, sizeof(RFLiCtrlBuf));
    readbuffer_(chan, mgr->buffer[chan], ch);
    return RFLGetAsyncStatus();
}

static RFLErrcode RFLLoadControllerAsync(s32 chan) {
    return RFLiLoadControllerAsync(chan, FALSE);
}

BOOL RFLiGetControllerData(RFLiCharInfo* info, s32 chan, u16 index,
                           BOOL allowHidden) {
    RFLiCtrlBufManager* mgr;
    RFLiCtrlBuf* buf;
    u16 mask;

    if (chan < 0 || chan >= 4) {
        return FALSE;
    }

    if (index >= RFL_CTRL_CHAR_MAX) {
        return FALSE;
    }

    mgr = RFLiGetCtrlBufManager();
    if (mgr == NULL) {
        return FALSE;
    }

    if (!mgr->loaded[chan]) {
        return FALSE;
    }

    buf = mgr->buffer[chan];
    mask = 1 << index;

    if (!RFLiIsValidID(&buf->data[index].createID)) {
        return FALSE;
    }

    if (!allowHidden && (buf->secretFlag & mask)) {
        return FALSE;
    }

    RFLiConvertRaw2Info(&buf->data[index], info);
    return TRUE;
}

static BOOL RFLIsAvailableControllerData(s32 chan, u16 index) {
    RFLiCtrlBufManager* mgr;
    RFLiCtrlBuf* buf;
    u16 mask;

    if (chan < 0 || chan >= 4) {
        // @bug Will be interpreted as TRUE
        return RFLErrcode_WrongParam;
    }

    if (index >= RFL_CTRL_CHAR_MAX) {
        // @bug Will be interpreted as TRUE
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return FALSE;
    }

    mgr = RFLiGetCtrlBufManager();
    if (mgr == NULL) {
        return FALSE;
    }

    if (!mgr->loaded[chan]) {
        return FALSE;
    }

    buf = mgr->buffer[chan];
    mask = 1 << index;

    if (buf->secretFlag & mask) {
        return FALSE;
    }

    return RFLiIsValidID(&buf->data[index].createID);
}
