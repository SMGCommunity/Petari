#include "Game/Scene/GameScenePauseControl.hpp"
#include "Game/Scene/GameScene.hpp"
#include "Game/Screen/GamePauseSequence.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/PauseButtonCheckerInGame.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"

namespace {
    NEW_NERVE(GameScenePauseControlNormal, GameScenePauseControl, Normal);
};

GameScenePauseControl::GameScenePauseControl(GameScene *pScene) : NerveExecutor("GameSceneポーズ制御") {
    mScene = pScene;
    mPauseChecker = nullptr;
    mPauseMenuOff = false;
    mPauseMenuNerve = nullptr;
    initNerve(&GameScenePauseControlNormal::sInstance);
    mPauseChecker = new PauseButtonCheckerInGame();
}

void GameScenePauseControl::registerNervePauseMenu(const Nerve *pNerve) {
    mPauseMenuNerve = pNerve;
}

void GameScenePauseControl::requestPauseMenuOff() {
    mPauseMenuOff = true;
}

void GameScenePauseControl::exeNormal() {
    tryStartPauseMenu();

    if (mPauseMenuOff) {
        AudWrap::getSystem()->exitPauseMenu();
        mScene->setNerveAfterPauseMenu();
        mPauseMenuOff = false;
        mScene->mPauseSeq->deactivate();
    }
}

bool GameScenePauseControl::tryStartPauseMenu() {
    if (mScene->isPermitToPauseMenu()) {
        mPauseChecker->update();

        if (mPauseChecker->isPermitToMinusPause()) {
            mScene->mPauseSeq->startPause(GamePauseSequence::ActivePause);
            mScene->setNerve(mPauseMenuNerve);
            return true;
        }
    
        if (mPauseChecker->isPermitToPlusPause()) {
            mScene->mPauseSeq->startPause(GamePauseSequence::ActivePause);
            mScene->setNerve(mPauseMenuNerve);
            return true;
        }
    }

    return false;
}

GameScenePauseControl::~GameScenePauseControl() {

}
