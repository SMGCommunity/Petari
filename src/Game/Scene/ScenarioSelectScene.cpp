#include "Game/Scene/ScenarioSelectScene.hpp"
#include "Game/Screen/CinemaFrame.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/ScenarioSelectLayout.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3DGraphBase/J3DDrawBuffer.hpp>

namespace NrvScenarioSelectScene {
    NERVE_DECL_EXE(ScenarioSelectSceneNrvDeactive, ScenarioSelectScene, Deactive);
    NERVE_DECL_NULL(ScenarioSelectSceneNrvInvalidScenarioSelect);
    NERVE_DECL_EXE(ScenarioSelectSceneNrvWaitStartScenarioSelect, ScenarioSelectScene, WaitStartScenarioSelect);
    NERVE_DECL_EXE(ScenarioSelectSceneNrvStartScenarioSelect, ScenarioSelectScene, StartScenarioSelect);
    NERVE_DECL_EXE(ScenarioSelectSceneNrvWaitScenarioSelect, ScenarioSelectScene, WaitScenarioSelect);
    NERVE_DECL_EXE(ScenarioSelectSceneNrvWaitResumeInitializeThread, ScenarioSelectScene, WaitResumeInitializeThread);
    NERVE_DECL_EXE(ScenarioSelectSceneNrvWaitInitializeEnd, ScenarioSelectScene, WaitInitializeEnd);
    NERVE_DECL_EXE(ScenarioSelectSceneNrvWaitDisappearLayout, ScenarioSelectScene, WaitDisappearLayout);
    NERVE_DECL_EXE(ScenarioSelectSceneNrvWaitResumeInitializeThreadIfRequestedReset, ScenarioSelectScene, WaitResumeInitializeThreadIfRequestedReset);
    NERVE_DECL_EXE(ScenarioSelectSceneNrvWaitResumeInitializeThreadIfCanceledSelect, ScenarioSelectScene, WaitResumeInitializeThreadIfCanceledSelect);
};

namespace {
    bool tryResumeInitializeThread();
};

ScenarioSelectScene::ScenarioSelectScene() : Scene("シナリオ選択シーン") {
    _14 = 0;
    _15 = 0;
    _16 = 0;
    mScenarioLayout = nullptr;
    mCinemaFrame = nullptr;
    _28 = 0;
    mEffectSystem = nullptr;
    mCameraContext = nullptr;
}

// ScenarioSelectScene::init

void ScenarioSelectScene::start() {
    _15 = 0;
    _16 = 0;
    setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvInvalidScenarioSelect::sInstance);
}

void ScenarioSelectScene::startBackground() {
    start();
    _15 = 1;
}

void ScenarioSelectScene::update() {
    if (_14) {
        updateNerve();
        bool res = false;
        if (isExecForeground() && _28 == 0) {
            res = true;
        }

        if (res) {
            mScenarioLayout->movement();
            mCinemaFrame->movement();
            MR::Effect::checkEffectSceneUpdate(mEffectSystem);
        }
    }
}

void ScenarioSelectScene::calcAnim() {
    bool res = false;
    if (isExecForeground() && _28 == 0) {
        res = true;
    }

    if (res) {
        mScenarioLayout->calcAnim();
        mCinemaFrame->calcAnim();
        MR::Effect::movementEffectNormal(mEffectSystem);
        calcViewAndEntry();
    }
}

void ScenarioSelectScene::calcViewAndEntry() {
    bool res = false;
    if (isExecForeground() && _28 == 0) {
        res = true;
    }

    if (res) {
        OSLockMutex(&MR::MutexHolder<0>::sMutex);
        setupCameraMtx();
        j3dSys._48 = _20;
        j3dSys._4C = _24;
        mScenarioLayout->calcViewAndEntryStarModel();
        OSUnlockMutex(&MR::MutexHolder<0>::sMutex);
    }
}

void ScenarioSelectScene::draw() const {
    bool res = false;
    if (isExecForeground() && _28 == 0) {
        res = true;
    }

    if (res) {
        OSLockMutex(&MR::MutexHolder<0>::sMutex);
        setupCameraMtx();
        j3dSys.drawInit();
        j3dSys._50 = 3;
        _20->draw();
        _20->frameInit();
        j3dSys._50 = 4;
        _24->draw();
        _24->frameInit();
        OSUnlockMutex(&MR::MutexHolder<0>::sMutex);
        MR::Effect::drawEffect3D(mEffectSystem, *mCameraContext->getViewMtx());
        mScenarioLayout->draw();
        MR::Effect::drawEffect2D(mEffectSystem);
        mCinemaFrame->draw();
    }
}

bool ScenarioSelectScene::isActive() const {
    bool ret = false;
    if (_14 && !isNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvDeactive::sInstance)) {
        ret = true;
    }

    return ret;
}

// ...

bool ScenarioSelectScene::isResetEnd() const {
    return _28 == 0;
}

/*
void ScenarioSelectScene::setupCameraMtx() const {
    PSMTXCopy(&j3dSys.mViewMtx, mCameraContext->getViewMtx());
}
*/

bool ScenarioSelectScene::trySetCurrentScenarioNo() const {
    if (mScenarioLayout->_28) {
        if (!MR::isScenarioDecided()) {
            s32 scenarioNo = mScenarioLayout->getSelectedScenarioNo();
            s32 placedNo = scenarioNo;
            GalaxyStatusAccessor accessor = MR::makeCurrentGalaxyStatusAccessor();
            if (accessor.isHiddenStar(scenarioNo)) {
                placedNo = MR::getPlacedHiddenStarScenarioNo(MR::getCurrentStageName(), scenarioNo);
            }
    
            MR::setCurrentScenarioNo(placedNo, scenarioNo);
        }
    
        return true;
    }

    return false;
}

void ScenarioSelectScene::suspend() {
    _16 = 0;
    if (!MR::isDead(mScenarioLayout)) {
        mScenarioLayout->kill();
    }

    if (!MR::isDead(mCinemaFrame)) {
        mCinemaFrame->kill();
    }

    _20->frameInit();
    _24->frameInit();
    MR::Effect::forceDeleteAllEmitters(mEffectSystem);
}

void ScenarioSelectScene::tryStartScreenToFrame() {
    if (!_15 && mScenarioLayout->isEndAnimStartStep()) {
        mCinemaFrame->tryScreenToFrame();
    }
}

void ScenarioSelectScene::exeDeactive() {
    if (MR::isFirstStep(this)) {
        if (!MR::isDead(mCinemaFrame)) {
            mCinemaFrame->kill();
        }
    }
}

void ScenarioSelectScene::exeStartScenarioSelect() {
    if (MR::isFirstStep(this)) {
        bool isSpecificStage = false;
        if (MR::isStageKoopaVs3() || MR::isEqualStageName("HeavensDoorGalaxy")) {
            isSpecificStage = true;
        }

        if (isSpecificStage) {
            MR::openSystemWipeFade(-1);
        }
        else {
            MR::openSystemWipeWhiteFade(-1);
        }

        mCinemaFrame->forceToFrame();
        mScenarioLayout->appear();
    }

    if (mScenarioLayout->isAppearStarEnd()) {
        setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvWaitScenarioSelect::sInstance);
    }
}

void ScenarioSelectScene::exeWaitScenarioSelect() {
    if (MR::isFirstStep(this)) {
        mCinemaFrame->tryFrameToScreen();
        MR::startStarPointerModeScenarioSelectScene(this);
    }

    if (trySetCurrentScenarioNo()) {
        MR::endStarPointerMode(this);
        setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvWaitResumeInitializeThread::sInstance);
    }
    else if (mScenarioLayout->isCanceled()) {
        MR::endStarPointerMode(this);
        setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvWaitResumeInitializeThreadIfCanceledSelect::sInstance);
    }
}

void ScenarioSelectScene::exeWaitResumeInitializeThread() {
    tryStartScreenToFrame();
    if (::tryResumeInitializeThread()) {
        setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvWaitInitializeEnd::sInstance);
    }
}

void ScenarioSelectScene::exeWaitInitializeEnd() {
    tryStartScreenToFrame();
    if (MR::isInitializeStateEnd()) {
        if (MR::isDead(mScenarioLayout)) {
            MR::Effect::forceDeleteAllEmitters(mEffectSystem);
            setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvDeactive::sInstance);
        }
        else if (mScenarioLayout->isReadyToDisappear()) {
            setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvWaitDisappearLayout::sInstance);
        }
    }
}

void ScenarioSelectScene::exeWaitDisappearLayout() {
    if (MR::isFirstStep(this)) {
        mCinemaFrame->forceToScreen();
        mScenarioLayout->disappear();
    }

    if (MR::isDead(mScenarioLayout)) {
        MR::Effect::forceDeleteAllEmitters(mEffectSystem);
        setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvDeactive::sInstance);
    }
}

void ScenarioSelectScene::exeWaitResumeInitializeThreadIfRequestedReset() {
    if (MR::isFirstStep(this)) {
        GameSystemFunction::resetCurrentScenarioNo();
    }

    if (::tryResumeInitializeThread()) {
        _28 = 0;
        setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvDeactive::sInstance);
    }
}

void ScenarioSelectScene::exeWaitResumeInitializeThreadIfCanceledSelect() {
    if (MR::isDead(mScenarioLayout)) {
        if (::tryResumeInitializeThread()) {
            GameSystemFunction::resetCurrentScenarioNo();
            GameSequenceFunction::requestCancelScenarioSelect();
            suspend();
            _16 = 1;
            MR::Effect::forceDeleteAllEmitters(mEffectSystem);
            setNerve(&NrvScenarioSelectScene::ScenarioSelectSceneNrvDeactive::sInstance);
        }
    }
}

namespace NrvScenarioSelectScene {
    INIT_NERVE(ScenarioSelectSceneNrvDeactive);
    INIT_NERVE(ScenarioSelectSceneNrvInvalidScenarioSelect);
    INIT_NERVE(ScenarioSelectSceneNrvWaitStartScenarioSelect);
    INIT_NERVE(ScenarioSelectSceneNrvStartScenarioSelect);
    INIT_NERVE(ScenarioSelectSceneNrvWaitScenarioSelect);
    INIT_NERVE(ScenarioSelectSceneNrvWaitResumeInitializeThread);
    INIT_NERVE(ScenarioSelectSceneNrvWaitInitializeEnd);
    INIT_NERVE(ScenarioSelectSceneNrvWaitDisappearLayout);
    INIT_NERVE(ScenarioSelectSceneNrvWaitResumeInitializeThreadIfRequestedReset);
    INIT_NERVE(ScenarioSelectSceneNrvWaitResumeInitializeThreadIfCanceledSelect);
};
