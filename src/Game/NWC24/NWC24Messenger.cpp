#include "Game/LiveActor/Nerve.hpp"
#include "Game/NWC24/NWC24Function.hpp"
#include "Game/NWC24/NWC24Messenger.hpp"
#include "Game/NWC24/NWC24System.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/System/GameSequenceDirector.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>

#define BACKGROUND_TASK_NUM 16

extern "C" int swprintf(wchar_t*, size_t, const wchar_t*, ...);

namespace {
    static const s32 sRetryMax = 10;

    NWC24Messenger* getNWC24Messenger() NO_INLINE {
        return SingletonHolder<GameSystem>::get()->mSequenceDirector->mMessenger;
    }
};

NWC24Messenger::NWC24Messenger(const char* pName) :
    NameObj(pName)
{
    mForegroundTask = new NWC24MessengerSub::SendTask();
    mBackgroundTaskArray = new NWC24MessengerSub::SendTask[BACKGROUND_TASK_NUM];
    mSendState = new NWC24MessengerSub::SendState(this);
    mSystem = new NWC24System(MR::getStationedHeapGDDR3(), 18);
    _1C = false;
}

void NWC24Messenger::initAfterResourceLoaded() {
    mSendState->initAfterResourceLoaded();
}

void NWC24Messenger::movement() {
    mSendState->updateNerve();
}

void NWC24Messenger::calcAnim() {
    mSendState->calcAnim();
}

void NWC24Messenger::draw() const {
    mSendState->draw();
}

void NWC24Messenger::send(
    const char* pTaskName,
    const wchar_t* pMessage,
    const wchar_t* pAltName,
    const u8* pPicture,
    u32 pictureSize,
    bool isBG,
    bool isMsgLedPattern,
    u16 tag,
    u8 delayHours)
{
    if (_1C) {
        return;
    }

    NWC24MessengerSub::SendTask* pTask;

    if (isBG) {
        int i;

        for (i = 0; i < BACKGROUND_TASK_NUM; i++) {
            if (!mBackgroundTaskArray[i]._0) {
                break;
            }
        }

        pTask = &mBackgroundTaskArray[i];
    }
    else {
        pTask = mForegroundTask;
    }

    if (pTask != nullptr) {
        pTask->_0 = true;
        pTask->mIsBG = isBG;
        pTask->_2 = false;
        pTask->mIsMsgLedPattern = isMsgLedPattern;
        pTask->mRetryNo = 0;
        pTask->mErr = NWC24_OK;
        pTask->mErrCode = 0;
        pTask->mTag = tag;
        pTask->mDelayHours = delayHours;
        pTask->mTaskName = pTaskName;
        pTask->mMessage = pMessage;
        pTask->mAltName = pAltName;
        pTask->mPicture = pPicture;
        pTask->mPictureSize = pictureSize;
    }
}

void NWC24Messenger::term(const char* pTaskName) {
    if (_1C) {
        return;
    }

    NWC24MessengerSub::SendTask* pTask = findTask(pTaskName);

    if (pTask == nullptr) {
        return;
    }

    pTask->_0 = false;
}

bool NWC24Messenger::isSent(const char* pTaskName) const {
    NWC24MessengerSub::SendTask* pTask = findTask(pTaskName);

    if (pTask != nullptr) {
        return pTask->_2;
    }

    return true;
}

bool NWC24Messenger::isError(const char* pTaskName) const {
    NWC24MessengerSub::SendTask* pTask = findTask(pTaskName);

    if (pTask != nullptr && pTask->_2) {
        return pTask->mErr != NWC24_OK;
    }

    return false;
}

void NWC24Messenger::prepareReset() {
    mForegroundTask->_0 = false;
    mForegroundTask->_2 = true;

    for (int i = 0; i < BACKGROUND_TASK_NUM; i++) {
        mBackgroundTaskArray[i]._0 = false;
        mBackgroundTaskArray[i]._2 = true;
    }

    mSendState->reset();

    _1C = true;
}

bool NWC24Messenger::isRunning() const {
    NWC24Err err;
    u32 value;

    return !mSystem->isSent(&err, &value);
}

void NWC24Messenger::reset() {
    NWC24Err err;
    mSystem->close(&err);

    _1C = false;
}

NWC24MessengerSub::SendTask* NWC24Messenger::findTask(const char* pTaskName) const {
    if (MR::isEqualString(mForegroundTask->mTaskName, pTaskName)
        && mForegroundTask->_0)
    {
        return mForegroundTask;
    }

    for (int i = 0; i < BACKGROUND_TASK_NUM; i++) {
        if (MR::isEqualString(mBackgroundTaskArray[i].mTaskName, pTaskName)
            && mBackgroundTaskArray[i]._0)
        {
            return &mBackgroundTaskArray[i];
        }
    }

    return nullptr;
}

NWC24MessengerSub::SendTask* NWC24Messenger::selectTask() const {
    NWC24MessengerSub::SendTask* pTask = nullptr;

    if (mForegroundTask->_0 && !mForegroundTask->_2) {
        pTask = mForegroundTask;
    }
    else {
        for (int i = 0; i < BACKGROUND_TASK_NUM; i++) {
            if (!mBackgroundTaskArray[i]._0) {
                continue;
            }

            if (mBackgroundTaskArray[i]._2) {
                continue;
            }

            pTask = &mBackgroundTaskArray[i];
            break;
        }
    }

    return pTask;
}

void NWC24Messenger::clearBackgroundTask() {
    for (int i = 0; i < BACKGROUND_TASK_NUM; i++) {
        mBackgroundTaskArray[i]._0 = false;
    }
}

namespace NWC24MessengerSub {
    namespace {
        NEW_NERVE(SendStateNrvWait, SendState, Wait);
        NEW_NERVE(SendStateNrvRetry, SendState, Retry);
        NEW_NERVE(SendStateNrvOpenBG, SendState, OpenBG);
        NEW_NERVE(SendStateNrvRunBG, SendState, RunBG);
        NEW_NERVE(SendStateNrvCloseBG, SendState, CloseBG);
        NEW_NERVE(SendStateNrvOpenFG, SendState, OpenFG);
        NEW_NERVE(SendStateNrvRunFG, SendState, RunFG);
        NEW_NERVE(SendStateNrvCloseFG, SendState, CloseFG);
        NEW_NERVE(SendStateNrvUpdateLimitFG, SendState, UpdateLimitFG);
        NEW_NERVE(SendStateNrvRetryErrorFG, SendState, RetryErrorFG);
        NEW_NERVE(SendStateNrvOpenErrorFG, SendState, OpenErrorFG);
        NEW_NERVE(SendStateNrvSendFatalErrorFG, SendState, SendFatalErrorFG);
        NEW_NERVE(SendStateNrvCloseErrorFG, SendState, CloseErrorFG);
        NEW_NERVE(SendStateNrvSendFullErrorFG, SendState, SendFullErrorFG);
        NEW_NERVE(SendStateNrvWaitCloseSysInfoMini, SendState, WaitCloseSysInfoMini);
    };

    SendState::SendState(NWC24Messenger* pHost) :
        NerveExecutor("送信状態"),
        mHost(pHost),
        mTask(nullptr),
        mWindow(nullptr),
        mMiniWindow(nullptr),
        _18(nullptr)
    {
        initNerve(&SendStateNrvWait::sInstance);
    }

    void SendState::initAfterResourceLoaded() {
        mWindow = MR::createSysInfoWindowExecuteWithChildren();
        mWindow->kill();

        mMiniWindow = MR::createSysInfoWindowMiniExecuteWithChildren();
        mMiniWindow->kill();
    }

    void SendState::updateNerve() {
        NerveExecutor::updateNerve();

        if (mWindow != nullptr) {
            mWindow->movement();
        }

        if (mMiniWindow != nullptr) {
            mMiniWindow->movement();
        }
    }

    void SendState::calcAnim() {
        if (mWindow != nullptr) {
            mWindow->calcAnim();
        }

        if (mMiniWindow != nullptr) {
            mMiniWindow->calcAnim();
        }
    }

    void SendState::draw() const {
        if (mWindow != nullptr) {
            mWindow->draw();
        }

        if (mMiniWindow != nullptr) {
            mMiniWindow->draw();
        }
    }

    void SendState::reset() {
        setNerve(&SendStateNrvWait::sInstance);

        mTask = nullptr;
        _18 = nullptr;

        if (mWindow != nullptr) {
            mWindow->kill();
        }

        if (mMiniWindow != nullptr) {
            mMiniWindow->kill();
        }
    }

    void SendState::exeWait() {
        if (selectTask() == nullptr) {
            return;
        }

        if (!mTask->mIsBG) {
            if (mTask->mRetryNo > 0 && isEndSysInfoMini()) {
                const char* pMessageId = "WiiMessageSending";

                if (mMiniWindow != nullptr) {
                    mMiniWindow->appear(pMessageId, SysInfoWindow::Type_Blocking, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
                }
            }

            setNerve(&SendStateNrvOpenFG::sInstance);
        }
        else {
            setNerve(&SendStateNrvOpenBG::sInstance);
        }
    }

    void SendState::exeRetry() {
        if (MR::isStep(this, 60)) {
            setNerve(&SendStateNrvWait::sInstance);
        }
    }

    void SendState::exeOpenBG() {
        if (!openSystem()) {
            if (isRestorableError()) {
                mTask->mRetryNo++;

                if (mTask->mRetryNo >= sRetryMax) {
                    mHost->clearBackgroundTask();
                    setNerve(&SendStateNrvWait::sInstance);
                }
                else {
                    setNerve(&SendStateNrvRetry::sInstance);
                }
            }
            else {
                mHost->clearBackgroundTask();
                setNerve(&SendStateNrvWait::sInstance);
            }
        }
        else {
            setNerve(&SendStateNrvRunBG::sInstance);
        }
    }

    void SendState::exeRunBG() {
        if (MR::isFirstStep(this)) {
            if (!send()) {
                return;
            }
        }

        if (!checkFinish()) {
            return;
        }

        if (mTask->mErr != NWC24_OK) {
            mHost->clearBackgroundTask();
            setNerve(&SendStateNrvWait::sInstance);
        }
        else {
            doneTask();

            if (selectTask() != nullptr) {
                if (!mTask->mIsBG) {
                    setNerve(&SendStateNrvRunFG::sInstance);
                }
                else {
                    setNerve(&SendStateNrvRunBG::sInstance);
                }
            }
            else {
                setNerve(&SendStateNrvCloseBG::sInstance);
            }
        }
    }

    void SendState::exeCloseBG() {
        closeSystem();
        mHost->clearBackgroundTask();
        setNerve(&SendStateNrvWait::sInstance);
    }

    void SendState::exeOpenFG() {
        if (!openSystem()) {
            if (isRestorableError()) {
                mTask->mRetryNo++;

                if (mTask->mRetryNo >= sRetryMax) {
                    setNerveAfterSysInfoWindowMiniDisappear(&SendStateNrvRetryErrorFG::sInstance);
                }
                else {
                    setNerve(&SendStateNrvRetry::sInstance);
                }
            }
            else {
                setNerveAfterSysInfoWindowMiniDisappear(&SendStateNrvOpenErrorFG::sInstance);
            }
        }
        else {
            setNerve(&SendStateNrvRunFG::sInstance);
        }
    }

    void SendState::exeRunFG() {
        if (MR::isFirstStep(this)) {
            if (!send()) {
                return;
            }
        }

        if (!checkFinish()) {
            return;
        }

        if (hasFatalErrorOccured()) {
            setNerveAfterSysInfoWindowMiniDisappear(&SendStateNrvSendFatalErrorFG::sInstance);
        }
        else {
            setNerve(&SendStateNrvCloseFG::sInstance);
        }
    }

    void SendState::exeCloseFG() {
        if (!closeSystem()) {
            setNerveAfterSysInfoWindowMiniDisappear(&SendStateNrvCloseErrorFG::sInstance);
        }
        else if (mTask->mErr != NWC24_OK) {
            setNerveAfterSysInfoWindowMiniDisappear(&SendStateNrvSendFullErrorFG::sInstance);
        }
        else {
            setNerveAfterSysInfoWindowMiniDisappear(&SendStateNrvUpdateLimitFG::sInstance);
        }
    }

    void SendState::exeUpdateLimitFG() {
        if (MR::isFirstStep(this)) {
            MR::updateWiiMailSentSize(mTask->mSentSize);
            GameSequenceFunction::startTotalMailSizeSaveSequence();
        }

        if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
            return;
        }

        doneTask();
        setNerve(&SendStateNrvWait::sInstance);
    }

    void SendState::exeRetryErrorFG() {
        if (MR::isFirstStep(this)) {
            const char* pMessageId = "WC24_04";

            if (mWindow != nullptr) {
                mWindow->appear(pMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
            }
        }

        if (MR::isDead(mWindow)) {
            doneTask();
            setNerve(&SendStateNrvWait::sInstance);
        }
    }

    void SendState::exeOpenErrorFG() {
        if (MR::isFirstStep(this)) {
            switch (mTask->mErr) {
            case NWC24_ERR_FATAL:
            case NWC24_ERR_BROKEN:
            case NWC24_ERR_FILE_OPEN:
            case NWC24_ERR_FILE_CLOSE:
            case NWC24_ERR_FILE_READ:
            case NWC24_ERR_FILE_WRITE:
            case NWC24_ERR_FILE_NOEXISTS:
            case NWC24_ERR_FILE_OTHER:
            case NWC24_ERR_INTERNAL_VF:
            case NWC24_ERR_FILE_BROKEN:
                appearSysInfo("WC24_01", mTask->mErrCode);
                break;
            case NWC24_ERR_NAND_CORRUPT:
                GameSequenceFunction::tryNANDErrorSequence(NAND_RESULT_CORRUPT);
                break;
            case NWC24_ERR_OLD_SYSTEM:
                appearSysInfo("WC24_02", mTask->mErrCode);
                break;
            case NWC24_ERR_INTERNAL_IPC:
                appearSysInfo("WC24_03", mTask->mErrCode);
                break;
            default:
                appearSysInfo("WC24_03", mTask->mErrCode);
                break;
            }
        }

        if (MR::isDead(mWindow)) {
            doneTask();
            setNerve(&SendStateNrvWait::sInstance);
        }
    }

    void SendState::exeSendFatalErrorFG() {
        if (MR::isFirstStep(this)) {
            const char* pMessageId;

            switch (mTask->mErr) {
            case NWC24_ERR_FILE_OPEN:
            case NWC24_ERR_FILE_CLOSE:
            case NWC24_ERR_FILE_READ:
            case NWC24_ERR_FILE_WRITE:
            case NWC24_ERR_FILE_NOEXISTS:
            case NWC24_ERR_FILE_OTHER:
            case NWC24_ERR_FILE_BROKEN:
                pMessageId = "WC24_09";

                if (mWindow != nullptr) {
                    mWindow->appear(pMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
                }
                break;
            case NWC24_ERR_NAND_CORRUPT:
                GameSequenceFunction::tryNANDErrorSequence(NAND_RESULT_CORRUPT);
                break;
            case NWC24_ERR_FULL:
                pMessageId = "WC24_08";

                if (mWindow != nullptr) {
                    mWindow->appear(pMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
                }
                break;
            default:
                pMessageId = "WC24_10";

                if (mWindow != nullptr) {
                    mWindow->appear(pMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
                }
                break;
            }
        }

        if (MR::isDead(mWindow)) {
            doneTask();
            setNerve(&SendStateNrvWait::sInstance);
        }
    }

    void SendState::exeCloseErrorFG() {
        if (MR::isFirstStep(this)) {
            const char* pMessageId;

            switch (mTask->mErr) {
            case NWC24_ERR_FILE_OPEN:
            case NWC24_ERR_FILE_CLOSE:
            case NWC24_ERR_FILE_READ:
            case NWC24_ERR_FILE_WRITE:
            case NWC24_ERR_FILE_NOEXISTS:
            case NWC24_ERR_FILE_OTHER:
            case NWC24_ERR_FILE_BROKEN:
                pMessageId = "WC24_10";

                if (mWindow != nullptr) {
                    mWindow->appear(pMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
                }
                break;
            default:
                pMessageId = "WC24_09";

                if (mWindow != nullptr) {
                    mWindow->appear(pMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
                }
                break;
            }
        }

        if (MR::isDead(mWindow)) {
            doneTask();
            setNerve(&SendStateNrvWait::sInstance);
        }
    }

    void SendState::exeSendFullErrorFG() {
        if (MR::isFirstStep(this)) {
            const char* pMessageId;

            switch (mTask->mErr) {
            case NWC24_ERR_FULL:
                pMessageId = "WC24_10";

                if (mWindow != nullptr) {
                    mWindow->appear(pMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
                }
                break;
            default:
                pMessageId = "WC24_SENDLIMIT";

                if (mWindow != nullptr) {
                    mWindow->appear(pMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
                }
                break;
            }
        }

        if (MR::isDead(mWindow)) {
            doneTask();
            setNerve(&SendStateNrvWait::sInstance);
        }
    }

    void SendState::exeWaitCloseSysInfoMini() {
        if (MR::isFirstStep(this)) {
            if (!isEndSysInfoMini()) {
                mMiniWindow->disappear();
            }
        }

        if (isEndSysInfoMini()) {
            setNerve(_18);
        }
    }

    bool SendState::selectTask() {
        mTask = mHost->selectTask();

        return mTask != nullptr;
    }

    bool SendState::openSystem() {
        return mHost->mSystem->open(&mTask->mErr, &mTask->mErrCode);
    }

    bool SendState::isRestorableError() const {
        NWC24Err err = mTask->mErr;

        return err == NWC24_ERR_MUTEX
            || err == NWC24_ERR_BUSY
            || err == NWC24_ERR_INPROGRESS;
    }

    bool SendState::send() {
        if (!mHost->mSystem->send(
            reinterpret_cast<const u16*>(mTask->mMessage),
            reinterpret_cast<const u16*>(mTask->mAltName),
            nullptr,
            0,
            mTask->mPicture,
            mTask->mPictureSize,
            mTask->mTag,
            mTask->mIsMsgLedPattern,
            mTask->mDelayHours))
        {
            setNerve(&SendStateNrvRetry::sInstance);

            return false;
        }

        return true;
    }

    bool SendState::checkFinish() {
        return mHost->mSystem->isSent(&mTask->mErr, &mTask->mSentSize);
    }

    bool SendState::closeSystem() {
        NWC24Err err;

        if (!mHost->mSystem->close(&err)) {
            mTask->mErr = err;

            return false;
        }

        return true;
    }

    bool SendState::hasFatalErrorOccured() const {
        return mTask->mErr != NWC24_OK
            && mTask->mErr != NWC24_ERR_FULL;
    }

    void SendState::doneTask() {
        mTask->_2 = true;

        if (mTask->mIsBG) {
            mTask->_0 = false;
        }
    }

    void SendState::appearSysInfo(const char* pParam1, s32 param2) {
        if (mWindow == nullptr) {
            return;
        }

        mWindow->appear(pParam1, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);

        if (param2 == 0) {
            return;
        }

        static wchar_t sMessage[32];
        swprintf(sMessage, sizeof(sMessage) / sizeof(*sMessage), L"%06d", -param2);

        mWindow->setTextBoxArgString(sMessage, 0);
    }

    bool SendState::isEndSysInfoMini() const {
        if (mMiniWindow != nullptr) {
            return MR::isDead(mMiniWindow);
        }

        return true;
    }

    void SendState::setNerveAfterSysInfoWindowMiniDisappear(const Nerve* pNerve) {
        if (isEndSysInfoMini()) {
            setNerve(pNerve);
        }
        else {
            _18 = pNerve;

            setNerve(&SendStateNrvWaitCloseSysInfoMini::sInstance);
        }
    }

    SendTask::SendTask() :
        _0(false),
        mIsBG(false),
        _2(false),
        mIsMsgLedPattern(false),
        mRetryNo(0),
        mErr(NWC24_OK),
        mErrCode(0),
        mSentSize(0),
        mTaskName(nullptr),
        mMessage(nullptr),
        mAltName(nullptr)
    {
        
    }
};

namespace MR {
    SendMailObj::SendMailObj(const char* pTaskName) :
        mTaskName(pTaskName),
        mMessage(nullptr),
        mSenderID(nullptr),
        mImage(nullptr),
        mImageSize(0),
        mIsBG(true),
        mIsLed(true),
        mTag(0),
        mDelay(0)
    {
        
    }

    void SendMailObj::setMessageDirect(const wchar_t* pMessage) {
        mMessage = pMessage;
    }

    void SendMailObj::setSenderID(const char* pMessageId) {
        mSenderID = getMailSender(pMessageId);
    }

    void SendMailObj::setImage(const u8* pImage, u32 imageSize) {
        mImage = pImage;
        mImageSize = imageSize;
    }

    void SendMailObj::setBGEnable() {
        mIsBG = true;
    }

    void SendMailObj::setBGDisable() {
        mIsBG = false;
    }

    void SendMailObj::setLedOff() {
        mIsLed = false;
    }

    void SendMailObj::setTag(u16 tag) {
        mTag = tag;
    }

    void SendMailObj::setDelay(u8 delay) {
        mDelay = delay;
    }

    void SendMailObj::send() {
        getNWC24Messenger()->send(mTaskName, mMessage, mSenderID, mImage, mImageSize, mIsBG, mIsLed, mTag, mDelay);
    }

    void termMail(const char* pTaskName) {
        getNWC24Messenger()->term(pTaskName);
    }

    bool isMailSent(const char* pTaskName) {
        return getNWC24Messenger()->isSent(pTaskName);
    }

    bool isMailErrorHappened(const char* pTaskName) {
        return getNWC24Messenger()->isError(pTaskName);
    }
};
