#include "Game/Scene/GameSceneScenarioOpeningCameraState.hpp"
#include "Game/Screen/ScenarioTitle.hpp"
#include "Game/Util.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/AudioLib/AudSystem.hpp"

namespace {
    NEW_NERVE(GameSceneScenarioOpeningCameraStateWait, GameSceneScenarioOpeningCameraState, Wait);
    NEW_NERVE(GameSceneScenarioOpeningCameraStatePlay, GameSceneScenarioOpeningCameraState, Play);
};

GameSceneScenarioOpeningCameraState::GameSceneScenarioOpeningCameraState() : NerveExecutor("シナリオ開始カメラ再生") {
    mBaseMtx.identity();
    initNerve(&GameSceneScenarioOpeningCameraStatePlay::sInstance);
    mScenarioTitle = new ScenarioTitle();
    mScenarioTitle->initWithoutIter();
    mScenarioTitle->kill();
}

void GameSceneScenarioOpeningCameraState::update() {
    updateNerve();
}

bool GameSceneScenarioOpeningCameraState::isDone() const {
    return isNerve(&GameSceneScenarioOpeningCameraStateWait::sInstance);
}

void GameSceneScenarioOpeningCameraState::start() {
    MR::stopSceneForScenarioOpeningCamera();
    MR::hidePlayer();
    MR::startStartAnimCamera();
    MR::startStarPointerModeDemo(this);
    MR::forceOpenWipeFade();
    MR::forceToFrameCinemaFrame();
    MR::startStageBGM("BGM_START_DEMO", false);
    PSMTXCopy(MR::getPlayerBaseMtx(), mBaseMtx);
    TVec3f namePos;
    if (MR::tryFindNamePos("スタートカメラマリオ座標", &namePos, nullptr)) {
        MR::setPlayerPos("スタートカメラマリオ座標");
    }
}

void GameSceneScenarioOpeningCameraState::end() {
    MR::stopStageBGM(0xA);
    MR::playSceneForScenarioOpeningCamera();
    MR::showPlayer();
    MR::endStartAnimCamera();
    MR::endStarPointerMode(this);
    mScenarioTitle->kill();
    MR::setPlayerBaseMtx(mBaseMtx);
}

void GameSceneScenarioOpeningCameraState::exePlay() {
    if (MR::isFirstStep(this)) {
        mScenarioTitle->start();
        MR::deactivateDefaultGameLayout();
        start();
    }

    if (trySkipTrigger()) {
        end();
        MR::forceToBlankCinemaFrame();
        setNerve(&GameSceneScenarioOpeningCameraStateWait::sInstance);
    }
    else {
        if (MR::isStep(this, MR::getStartAnimCameraFrame() - 60)) {
            MR::tryFrameToBlankCinemaFrame();
            mScenarioTitle->end();
        }

        if (MR::isStep(this, MR::getStartAnimCameraFrame() - 20)) {
            AudWrap::getSystem()->setVar(30);
        }

        if (MR::isStartAnimCameraEnd() && MR::isStopCinemaFrame()) {
            end();
            setNerve(&GameSceneScenarioOpeningCameraStateWait::sInstance);
        }
    }
}

bool GameSceneScenarioOpeningCameraState::trySkipTrigger() const {
    if (MR::isFirstStep(this)) {
        return false;
    }

    return !MR::isAlreadyVisitedCurrentStageAndScenario() ? false :  MR::testSystemPadTriggerDecide(); 
}

GameSceneScenarioOpeningCameraState::~GameSceneScenarioOpeningCameraState() {

}

void GameSceneScenarioOpeningCameraState::exeWait() {
    
}
