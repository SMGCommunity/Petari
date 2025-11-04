#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/DrawSyncManager.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/GameSystemResetAndPowerProcess.hpp"
#include "Game/System/MainLoopFramework.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/TriggerChecker.hpp"
#include "Game/Util/ValueControl.hpp"
#include "Game/SingletonHolder.hpp"

namespace {
    static const s32 sFadeinoutFrame = 30;
    static const s32 sResetWaitFrame = 15;
};

namespace NrvGameSystemResetAndPowerProcess {
    NEW_NERVE(GameSystemResetAndPowerProcessPolling, GameSystemResetAndPowerProcess, Polling);
    NEW_NERVE(GameSystemResetAndPowerProcessWaitResetPermitted, GameSystemResetAndPowerProcess, WaitResetPermitted);
    NEW_NERVE(GameSystemResetAndPowerProcessPrepareReset, GameSystemResetAndPowerProcess, PrepareReset);
    NEW_NERVE(GameSystemResetAndPowerProcessReset, GameSystemResetAndPowerProcess, Reset);
    NEW_NERVE(GameSystemResetAndPowerProcessWaitPrepareFadein, GameSystemResetAndPowerProcess, WaitPrepareFadein);
    NEW_NERVE(GameSystemResetAndPowerProcessFadein, GameSystemResetAndPowerProcess, Fadein);
};

void GameSystemResetAndPowerProcess::init(const JMapInfoIter& rIter) {
    initNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessPolling::sInstance);
    OSSetPowerCallback(GameSystemResetAndPowerProcess::handleOSPowerCallback);
    appear();
}

// GameSystemResetAndPowerProcess::draw

bool GameSystemResetAndPowerProcess::isActive() const {
    return !isNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessPolling::sInstance);
}

void GameSystemResetAndPowerProcess::setResetOperationApplicationReset() {
    mResetOperation = 0;
}

void GameSystemResetAndPowerProcess::setResetOperationReturnToMenu() {
    mResetOperation = 2;
}

void GameSystemResetAndPowerProcess::requestReset(bool param1) {
    if (tryPermitReset()) {
        _5E = param1;

        setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessWaitResetPermitted::sInstance);
    }
}

void GameSystemResetAndPowerProcess::requestGoWiiMenu(bool param1) {
    if (tryPermitReset()) {
        _5E = param1;
        setResetOperationReturnToMenu();
        setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessWaitResetPermitted::sInstance);
    }
}

void GameSystemResetAndPowerProcess::notifyCheckDiskResult(bool param1) {
    _5C = true;

    if (param1) {
        return;
    }

    if (mResetOperation == 0) {
        setResetOperationReturnToMenu();
    }
}

void GameSystemResetAndPowerProcess::exePolling() {
    if (tryAcceptPowerOff()) {
        setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessWaitResetPermitted::sInstance);
    }
    else if (mResetTriggerChecker->getOnTrigger()) {
        setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessWaitResetPermitted::sInstance);
    }
}

void GameSystemResetAndPowerProcess::exeWaitResetPermitted() {
    if (GameSystemFunction::isPermitToResetAudioSystem()
        && GameSystemFunction::isPermitToResetSaveDataHandleSequence())
    {
        if (_5E) {
            mFadeinValueControl->setZero();
        }
        else {
            mFadeinValueControl->setDirToZero();
        }

        _5E = false;

        GameSystemFunction::activateScreenPreserver();
        setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessPrepareReset::sInstance);
    }
}

void GameSystemResetAndPowerProcess::exePrepareReset() {
    if (MR::isFirstStep(this)) {
        GameSystemFunction::prepareResetAudioSystem();
        GameSystemFunction::prepareResetSystem();
        GameSystemFunction::resetAllControllerRumble();
        GameSystemFunction::forceToDeactivateHomeButtonLayout();
        GameSequenceFunction::requestPrepareResetNWC24();
        GameSystemFunction::prepareResetSaveDataHandleSequence();
    }

    if (MR::isStep(this, 2)) {
        GameSystemFunction::requestResetAudioSystem(mResetOperation != 0);
    }

    bool b = mFadeinValueControl->mFrame == 0;

    if (b) {
        b = GameSystemFunction::isPrepareResetSaveDataHandleSequence();
    }

    if (b) {
        b = isResetAcceptAudio();
    }

    if (b) {
        b = GameSequenceFunction::isEnableToResetNWC24();
    }

    if (b) {
        GameSequenceFunction::resetNWC24();
        setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessReset::sInstance);
    }
}

void GameSystemResetAndPowerProcess::exeReset() {
    if (MR::isFirstStep(this)) {
        _5C = false;

        DVDCheckDiskAsync(&mCommandBlock, GameSystemResetAndPowerProcess::handleCheckDiskAsync);
    }

    if (_5C) {
        GameSystemFunction::setPermissionToCheckWiiRemoteConnectAndScreenDimming(false);

        if (mResetOperation != 0) {
            exitApplication();
        }
    }

    MR::setNerveAtStep(this, &NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessWaitPrepareFadein::sInstance, sResetWaitFrame);
}

void GameSystemResetAndPowerProcess::exeWaitPrepareFadein() {
    if (MR::isFirstStep(this)) {
        GameSystemFunction::deactivateScreenPreserver();
        GameSystemFunction::resumeResetAudioSystem();
        MR::forceOpenSystemWipeFade();
    }

    if (GameSystemFunction::isPreparedFadeinSystem()) {
        setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessFadein::sInstance);
    }
}

void GameSystemResetAndPowerProcess::exeFadein() {
    if (MR::isFirstStep(this)) {
        GameSystemFunction::restoreFromResetSaveDataHandleSequence();
    }

    if (tryAcceptPowerOff()) {
        GameSystemFunction::restartControllerLeaveWatcher();
        GameSystemFunction::restartSceneController();
        setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessWaitResetPermitted::sInstance);
    }
    else {
        if (MR::isFirstStep(this)) {
            mFadeinValueControl->setDirToOneResetFrame();
        }

        if (mFadeinValueControl->mFrame == mFadeinValueControl->mMaxFrame) {
            GameSystemFunction::restartControllerLeaveWatcher();
            GameSystemFunction::restartSceneController();
            setNerve(&NrvGameSystemResetAndPowerProcess::GameSystemResetAndPowerProcessPolling::sInstance);
        }
    }
}

void GameSystemResetAndPowerProcess::exitApplication() {
    DrawSyncManager::end();
    MainLoopFramework::setForOSResetSystem();
    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();

    switch (mResetOperation) {
    case 1:
        OSRestart(0);
        break;
    case 2:
        OSReturnToMenu();
        break;
    case 4:
        OSRebootSystem();
        break;
    case 3:
        OSShutdownSystem();
        break;
    }
}

bool GameSystemResetAndPowerProcess::tryPermitReset() {
    return !isActive();
}

bool GameSystemResetAndPowerProcess::tryAcceptPowerOff() {
    if (!mIsValidPowerOff) {
        return false;
    }

    mIsValidPowerOff = false;
    mResetOperation = 3;

    return true;
}

bool GameSystemResetAndPowerProcess::isResetAcceptAudio() const {
    if (mResetOperation != 0
        && MR::isGreaterStep(this, 2)
        && GameSystemFunction::isDoneResetAudioSystem())
    {
        return true;
    }

    return MR::isGreaterStep(this, 17);
}

void GameSystemResetAndPowerProcess::control() {
    mResetTriggerChecker->update(OSGetResetButtonState() != FALSE);
    mFadeinValueControl->update();
}

void GameSystemResetAndPowerProcess::handleOSPowerCallback() {
    SingletonHolder<GameSystemResetAndPowerProcess>::get()->mIsValidPowerOff = true;
}

void GameSystemResetAndPowerProcess::handleCheckDiskAsync(s32 result, DVDCommandBlock* pBlock) {
    SingletonHolder<GameSystemResetAndPowerProcess>::get()->notifyCheckDiskResult(result != 0);
}

GameSystemResetAndPowerProcess::GameSystemResetAndPowerProcess() :
    LayoutActor("リセット・電源", false),
    mResetTriggerChecker(nullptr),
    mFadeinValueControl(nullptr),
    mResetOperation(1),
    _5C(true),
    mIsValidPowerOff(false),
    _5E(false)
{
    mResetTriggerChecker = new TriggerChecker();

    mFadeinValueControl = new ValueControl(sFadeinoutFrame);
    mFadeinValueControl->setOne();
}
