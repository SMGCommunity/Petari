#include "Game/Screen/GamePauseSequence.hpp"
#include "Game/Screen/PauseMenu.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/GalaxyMapController.hpp"

namespace {
    NEW_NERVE(GamePauseSequenceDeactive, GamePauseSequence, Deactive);
    NEW_NERVE(GamePauseSequenceActivePauseMenu, GamePauseSequence, ActivePauseMenu);
    NEW_NERVE(GamePauseSequenceActivePowerStarList, GamePauseSequence, ActivePowerStarList);
    NEW_NERVE(GamePauseSequenceSceneInformation, GamePauseSequence, SceneInformation);
};

GamePauseSequence::GamePauseSequence() : LayoutActor("ポーズ画面管理", true) {
    mMenuType = ActivePause;
    mPauseMenu = nullptr;
    mWindowMenuFunc = nullptr;
}

void GamePauseSequence::init(const JMapInfoIter &) {
    if (!MR::isStageDisablePauseMenu()) {
        mPauseMenu = new PauseMenu();
        mPauseMenu->initWithoutIter();
    }

    initNerve(&GamePauseSequenceDeactive::sInstance);
}

void GamePauseSequence::initWindowMenu(const MR::FunctorBase &rFunc) {
    mWindowMenuFunc = rFunc.clone(nullptr);
}

void GamePauseSequence::startPause(MenuType type) {
    AudWrap::getSystem()->enterPauseMenu();
    mMenuType = type;
    appear();
    MR::startStarPointerModePauseMenu(this);
    GameSystemFunction::onPauseBeginAllRumble();
    MR::startSystemSE("SE_SY_PAUSE_ON", -1, -1);
    MR::startCSSound("CS_CLICK_OPEN", nullptr, 0);

    switch (mMenuType) {
        case ActivePause:
            mPauseMenu->appear();
            setNerve(&GamePauseSequenceActivePauseMenu::sInstance);
            break;
        case SceneInformation:
            setNerve(&GamePauseSequenceSceneInformation::sInstance);
            break;
    }
}

void GamePauseSequence::deactivate() {
    MR::endStarPointerMode(this);
    kill();
    GameSystemFunction::onPauseEndAllRumble();
    setNerve(&GamePauseSequenceDeactive::sInstance);
}

void GamePauseSequence::exeActivePauseMenu() {
    if (MR::isDead(mPauseMenu)) {
        (*mWindowMenuFunc)();
    }
}

void GamePauseSequence::exeActivePowerStarList() {
    MR::isFirstStep(this);

    if (!MR::isActiveGalaxyMapLayout()) {
        (*mWindowMenuFunc)();
    }
}

void GamePauseSequence::exeSceneInformation() {
    
}

void GamePauseSequence::exeDeactive() {

}

GamePauseSequence::~GamePauseSequence() {
    
}
