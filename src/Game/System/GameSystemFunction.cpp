#include "Game/System/GameSystemFunction.hpp"
#include "Game/NameObj/NameObjRegister.hpp"
#include "Game/Scene/LogoScene.hpp"
#include "Game/Screen/HomeButtonLayout.hpp"
#include "Game/Screen/ScreenPreserver.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/AudSystemWrapper.hpp"
#include "Game/System/GameSequenceDirector.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemDimmingWatcher.hpp"
#include "Game/System/GameSystemErrorWatcher.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/GameSystemResetAndPowerProcess.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/GameSystemStationedArchiveLoader.hpp"
#include "Game/System/SaveDataHandleSequence.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPadRumble.hpp"
#include "Game/Util/GamePadUtil.hpp"

namespace {
    LogoScene* getLogoScene() {
        LogoScene* pLogoScene = static_cast< LogoScene* >(SingletonHolder< GameSystem >::get()->mSceneController->mScene);

        if (pLogoScene == nullptr) {
            return nullptr;
        }

        return pLogoScene;
    }

    AudSystemWrapper* getAudSystemWrapper() NO_INLINE { return SingletonHolder< GameSystem >::get()->mObjHolder->mSysWrapper; }

    SaveDataHandleSequence* getSaveDataHandleSequence() NO_INLINE {
        return SingletonHolder< GameSystem >::get()->mSequenceDirector->mSaveDataHandleSequence;
    }
};  // namespace

namespace GameSystemFunction {
    void loadAudioStaticWaveData() { getAudSystemWrapper()->loadStaticWaveData(); }

    bool isLoadedAudioStaticWaveData() { return getAudSystemWrapper()->isLoadDoneStaticWaveData(); }

    void initAfterStationedResourceLoaded() { SingletonHolder< GameSystem >::get()->initAfterStationedResourceLoaded(); }

    void setSceneNameObjHolderToNameObjRegister() {
        SingletonHolder< NameObjRegister >::get()->setCurrentHolder(SingletonHolder< GameSystem >::get()->mSceneController->mObjHolder);
    }

    bool isCreatedGameDataHolder() { return SingletonHolder< GameSystem >::get()->mSequenceDirector->isInitializedGameDataHolder(); }

    bool isCreatedSystemWipe() { return SingletonHolder< GameSystem >::get()->mSystemWipeHolder != nullptr; }

    bool isDoneLoadSystemArchive() { return SingletonHolder< GameSystem >::get()->isDoneLoadSystemArchive(); }

    bool tryToLoadSystemArchive() {
        bool isExecuteLoadSystemArchive = SingletonHolder< GameSystem >::get()->isExecuteLoadSystemArchive();

        if (!isExecuteLoadSystemArchive) {
            SingletonHolder< GameSystem >::get()->startToLoadSystemArchive();
        }

        return !isExecuteLoadSystemArchive;
    }

    void requestChangeArchivePlayer(bool isPlayerMario) {
        SingletonHolder< GameSystem >::get()->mStationedArchiveLoader->requestChangeArchivePlayer(isPlayerMario);
    }

    bool isEndChangeArchivePlayer() { return SingletonHolder< GameSystem >::get()->mStationedArchiveLoader->isDone(); }

    void activateScreenPreserver() { SingletonHolder< GameSystem >::get()->mObjHolder->mScreenPreserver->activate(); }

    void deactivateScreenPreserver() { SingletonHolder< GameSystem >::get()->mObjHolder->mScreenPreserver->deactivate(); }

    bool isOccurredSystemWarning() { return SingletonHolder< GameSystem >::get()->mErrorWatcher->isWarning(); }

    bool isResetProcessing() { return SingletonHolder< GameSystemResetAndPowerProcess >::get()->isActive(); }

    void setResetOperationApplicationReset() { SingletonHolder< GameSystemResetAndPowerProcess >::get()->setResetOperationApplicationReset(); }

    void setResetOperationReturnToMenu() { SingletonHolder< GameSystemResetAndPowerProcess >::get()->setResetOperationReturnToMenu(); }

    void requestResetGameSystem(bool param1) { SingletonHolder< GameSystemResetAndPowerProcess >::get()->requestReset(param1); }

    void requestGoWiiMenu(bool param1) { SingletonHolder< GameSystemResetAndPowerProcess >::get()->requestGoWiiMenu(param1); }

    void forceToDeactivateHomeButtonLayout() {
        HomeButtonLayout* pHomeButtonLayout = SingletonHolder< GameSystem >::get()->mHomeButtonLayout;

        if (pHomeButtonLayout != nullptr) {
            pHomeButtonLayout->forceToDeactive();
        }
    }

    void resetCurrentScenarioNo() { SingletonHolder< GameSystem >::get()->mSceneController->resetCurrentScenarioNo(); }

    bool isPermitToResetSaveDataHandleSequence() {
        if (getSaveDataHandleSequence() != nullptr) {
            return getSaveDataHandleSequence()->isPermitToReset();
        }

        return true;
    }

    void prepareResetSaveDataHandleSequence() {
        if (getSaveDataHandleSequence() != nullptr) {
            getSaveDataHandleSequence()->prepareReset();
        }
    }

    bool isPrepareResetSaveDataHandleSequence() {
        if (getSaveDataHandleSequence() != nullptr) {
            return getSaveDataHandleSequence()->isPreparedReset();
        }

        return true;
    }

    void restoreFromResetSaveDataHandleSequence() {
        if (getSaveDataHandleSequence() != nullptr) {
            getSaveDataHandleSequence()->restoreFromReset();
        }
    }

    bool isPermitToResetAudioSystem() { return getAudSystemWrapper()->isPermitToReset(); }

    void prepareResetAudioSystem() { getAudSystemWrapper()->prepareReset(); }

    void requestResetAudioSystem(bool param1) { getAudSystemWrapper()->requestReset(param1); }

    bool isDoneResetAudioSystem() { return getAudSystemWrapper()->isResetDone(); }

    void resumeResetAudioSystem() { getAudSystemWrapper()->resumeReset(); }

    void stopControllerLeaveWatcher() {
        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            MR::getWPad(chan)->stopLeaveWatcher();
        }
    }

    void startControllerLeaveWatcher() {
        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            MR::getWPad(chan)->startLeaveWatcher();
        }
    }

    void restartControllerLeaveWatcher() {
        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            MR::getWPad(chan)->restartLeaveWatcher();
        }
    }

    void setPadConnectCallback() { WPadHolder::setConnectCallback(); }

    void resetAllControllerRumble() {
        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            MR::getWPad(chan)->_18->registInstance();
            MR::getWPad(chan)->_18->stop();
        }
    }

    void setAutoSleepTimeWiiRemote(bool isLongAutoSleep) { MR::setAutoSleepTimeWiiRemote(isLongAutoSleep); }

    bool setPermissionToCheckWiiRemoteConnectAndScreenDimming(bool permission) {
        bool permissionPrev = SingletonHolder< GameSystem >::get()->mErrorWatcher->setPermissionUpdateWiiRemoteStatus(permission);

        SingletonHolder< GameSystem >::get()->mDimmingWatcher->_4 = permission;

        return permissionPrev;
    }

    void onPauseBeginAllRumble() {
        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            MR::getWPad(chan)->_18->stop();
            MR::getWPad(chan)->_1C->registInstance();
            MR::getWPad(chan)->_1C->stop();
        }
    }

    void onPauseEndAllRumble() {
        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            MR::getWPad(chan)->_18->registInstance();
            MR::getWPad(chan)->_1C->stop();
        }
    }

    void onHomeButtonMenuBeginAllRumble() {
        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            MR::getWPad(chan)->_18->pause();
            MR::getWPad(chan)->_1C->registInstance();
            MR::getWPad(chan)->_1C->stop();
        }
    }

    void onHomeButtonMenuCloseAllRumble() {
        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            MR::getWPad(chan)->_18->registInstance();
            MR::getWPad(chan)->_1C->stop();
        }
    }

    void onHomeButtonMenuEndAllRumble() {
        WPadRumble* pRumble;

        for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + MR::getWPadMaxCount(); chan++) {
            pRumble = MR::getWPad(chan)->_18;
            pRumble->_BC = 200;
            pRumble->stop();

            pRumble = MR::getWPad(chan)->_1C;
            pRumble->_BC = 200;
            pRumble->stop();
        }
    }

    void prepareResetSystem() {
        SingletonHolder< GameSystem >::get()->prepareReset();
        SingletonHolder< GameSystem >::get()->mSceneController->prepareReset();
    }

    bool isPreparedFadeinSystem() {
        return SingletonHolder< GameSystem >::get()->isPreparedReset() && SingletonHolder< GameSystem >::get()->mSceneController->isPreparedReset();
    }

    void restartSceneController() { SingletonHolder< GameSystem >::get()->mSceneController->restartGameAfterResetting(); }

    bool isDisplayStrapRemineder() { return MR::isEqualSceneName("Logo") && getLogoScene() != nullptr && getLogoScene()->isDisplayStrapRemineder(); }
};  // namespace GameSystemFunction
