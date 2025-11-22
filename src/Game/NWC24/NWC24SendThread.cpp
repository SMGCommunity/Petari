#include "Game/NWC24/NWC24Function.hpp"
#include "Game/NWC24/NWC24SendThread.hpp"
// #include "Game/NWC24/UTF16Util.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>

#define STACK_SIZE 0x8000

NWC24SendThread::NWC24SendThread(s32 priority, JKRHeap* pHeap) {
    if (pHeap == nullptr) {
        pHeap = MR::getCurrentHeap();
    }

    mMessage = new (pHeap, 0) OSMessage[mMessageMax];

    initMsgSendStatus();
    OSInitMessageQueue(&mMessageQueue, &mMessage, mMessageMax);

    u8* pStackBase = new (pHeap, 0) u8[STACK_SIZE];

    OSCreateThread(
        this,
        &NWC24SendThread::threadProc,
        nullptr,
        &pStackBase[STACK_SIZE],
        STACK_SIZE,
        priority,
        1 /* OS_THREAD_ATTR_DETACH */);
    OSResumeThread(this);
}

bool NWC24SendThread::requestSend(
    const u16* pText,
    const u16* pAltName,
    const u8*  pLetter,
    u32        letterSize,
    const u8*  pPicture,
    u32        pictureSize,
    u16        tag,
    bool       isMsgLedPattern,
    u8         delayHours) {
    BOOL status = OSDisableInterrupts();

    if (mMessageQueue.usedCount >= mMessageQueue.msgCount) {
        OSRestoreInterrupts(status);

        return false;
    }

    mMsgSendStatus.mText = pText;
    mMsgSendStatus.mAltName = pAltName;
    mMsgSendStatus.mLetter = pLetter;
    mMsgSendStatus.mLetterSize = letterSize;
    mMsgSendStatus.mPicture = pPicture;
    mMsgSendStatus.mPictureSize = pictureSize;
    mMsgSendStatus._18 = false;
    mMsgSendStatus.mIsMsgLedPattern = isMsgLedPattern;
    mMsgSendStatus.mSentErr = NWC24_OK;
    mMsgSendStatus.mSentSize = 0;
    mMsgSendStatus.mTag = tag;
    mMsgSendStatus.mDelayHours = delayHours;

    OSSendMessage(&mMessageQueue, &mMsgSendStatus, OS_MESSAGE_NOBLOCK);
    OSRestoreInterrupts(status);

    return true;
}

bool NWC24SendThread::isDone(NWC24Err* pErr, u32* pSize) {
    BOOL status = OSDisableInterrupts();

    if (mMsgSendStatus._18) {
        *pErr = mMsgSendStatus.mSentErr;
        *pSize = mMsgSendStatus.mSentSize;

        OSRestoreInterrupts(status);

        return true;
    } else {
        *pErr = NWC24_OK;

        OSRestoreInterrupts(status);

        return false;
    }
}

void* NWC24SendThread::threadProc(void*) {
    while (true) {
        MsgSendStatus* pMsgSendStatus = nullptr;
        OSReceiveMessage(&mMessageQueue, reinterpret_cast< void** >(&pMsgSendStatus), OS_MESSAGE_BLOCK);

        pMsgSendStatus->mSentErr = sendMessage(pMsgSendStatus, &pMsgSendStatus->mSentSize);
        pMsgSendStatus->_18 = true;
    }
}

void NWC24SendThread::initMsgSendStatus() {
    mMsgSendStatus.mText = nullptr;
    mMsgSendStatus.mAltName = nullptr;
    mMsgSendStatus.mLetter = nullptr;
    mMsgSendStatus.mLetterSize = 0;
    mMsgSendStatus.mPicture = nullptr;
    mMsgSendStatus.mPictureSize = 0;
    mMsgSendStatus._18 = true;
    mMsgSendStatus.mIsMsgLedPattern = false;
    mMsgSendStatus.mSentErr = NWC24_OK;
    mMsgSendStatus.mTag = 0;
    mMsgSendStatus.mDelayHours = 0;
}

/*
TODO: Define all of the NWC24 API.
NWC24Err NWC24SendThread::sendMessage(MsgSendStatus* pMsgSendStatus, u32* pMsgSize) {
    NWC24Err err;
    NWC24MsgObj msgObj;

    err = NWC24InitMsgObj(&msgObj, NWC24_MSGTYPE_RVL_APP);

    if (err != NWC24_OK) {
        return err;
    }

    err = setToMyself(&msgObj);

    if (err != NWC24_OK) {
        return err;
    }

    if (pMsgSendStatus->mAltName != nullptr) {
        err = NWC24SetMsgAltName(&msgObj, pMsgSendStatus->mAltName, MR::strlenUTF16(pMsgSendStatus->mAltName));

        if (err != NWC24_OK) {
            return err;
        }
    }

    err = NWC24SetMsgTag(&msgObj, pMsgSendStatus->mTag);

    if (err != NWC24_OK) {
        return err;
    }

    if (pMsgSendStatus->mDelayHours != 0) {
        err = NWC24SetMsgMBDelay(&msgObj, pMsgSendStatus->mDelayHours);

        if (err != NWC24_OK) {
            return err;
        }
    }

    err = NWC24SetMsgText(&msgObj, pMsgSendStatus->mText, MR::strlenUTF16(pMsgSendStatus->mText) * sizeof(u16), NWC24_UTF_16, NWC24_ENC_8BIT);

    if (err != NWC24_OK) {
        return err;
    }

    if (pMsgSendStatus->mLetter != nullptr) {
        err = NWC24SetMsgAttached(&msgObj, pMsgSendStatus->mLetter, pMsgSendStatus->mLetterSize, NWC24_APP_WII_MSGBOARD);

        if (err != NWC24_OK) {
            return err;
        }
    }

    if (pMsgSendStatus->mPicture != nullptr) {
        err = NWC24SetMsgAttached(&msgObj, pMsgSendStatus->mPicture, pMsgSendStatus->mPictureSize, NWC24_IMG_WII_PICTURE);

        if (err != NWC24_OK) {
            return err;
        }
    }

    if (pMsgSendStatus->mIsMsgLedPattern) {
        err = NWC24SetMsgLedPattern(&msgObj, NWC24_LED_APP_DEFAULT);
    }

    err = NWC24SetMsgMBNoReply(&msgObj, true);

    if (err != NWC24_OK) {
        return err;
    }

    if (!checkTotalSize(pMsgSendStatus)) {
        return NWC24_ERR_FULL;
    }

    err = NWC24CommitMsg(&msgObj);

    NWC24GetMsgSize(&msgObj, pMsgSize);

    return err;
}
*/

bool NWC24SendThread::checkTotalSize(MsgSendStatus* pMsgSendStatus) {
    u32 letterSize;
    u32 pictureSize;

    if (pMsgSendStatus->mLetter == nullptr) {
        letterSize = 0;
    } else {
        letterSize = pMsgSendStatus->mLetterSize;
    }

    if (pMsgSendStatus->mPicture == nullptr) {
        pictureSize = 0;
    } else {
        pictureSize = pMsgSendStatus->mPictureSize;
    }

    return MR::checkWiiMailLimit(
        MR::calcWiiMailSize(
            pMsgSendStatus->mAltName,
            pMsgSendStatus->mText,
            pictureSize,
            letterSize));
}

/*
TODO: Define all of the NWC24 API.
NWC24Err NWC24SendThread::setToMyself(NWC24MsgObj* pMsgObj) {
    NWC24Err err;
    NWC24UserId userId;

    err = NWC24GetMyUserId(&userId);

    if (err == NWC24_OK) {
        return NWC24SetMsgToId(pMsgObj, userId);
    }

    return err;
}
*/
