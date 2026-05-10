#include "Game/Map/ScenarioSelectStar.hpp"
#include "Game/MapObj/PowerStar.hpp"

namespace {
    static const s32 cAppearWaitFrame = 60;
    static const s32 cAppearDelayFrame = 15;
    static const s32 cHideDelayFrame = 10;
    static const f32 cNotPointingRotateSpeedZ = 1.5f;
    static const f32 cPointingRotateSpeedZ = 3.0f;
    static const f32 cPointingScale = 1.5f;
    static const f32 cNotPointingScale = 0.9f;
    static const s32 cMoveInterval = 180;
    static const f32 cMoveDistance = 15.0f;
    static const s32 cAppearFrame = 60;
    static const s32 cStartPointingFrame = 20;
    static const s32 cEndPointingFrame = 10;
    static const s32 cSelectedWaitFrame = 40;
    static const s32 cSelectedMoveFrame = 60;
    static const f32 cSelectedRotateSpeedZ = 20.0f;
    static const f32 cSelectedRotateMoveRate = 6.0f;
    static const s32 cSelectedRotateAccelFrame = 180;
}  // namespace

namespace NrvScenarioSelectStar {
    NEW_NERVE(ScenarioSelectStarNrvAppear, ScenarioSelectStar, Appear);
    NEW_NERVE(ScenarioSelectStarNrvNotPointing, ScenarioSelectStar, NotPointing);
    NEW_NERVE(ScenarioSelectStarNrvPointing, ScenarioSelectStar, Pointing);
    NEW_NERVE(ScenarioSelectStarNrvEndPointing, ScenarioSelectStar, EndPointing);
    NEW_NERVE(ScenarioSelectStarNrvSelected, ScenarioSelectStar, Selected);
    NEW_NERVE(ScenarioSelectStarNrvSelectedMove, ScenarioSelectStar, SelectedMove);
    NEW_NERVE(ScenarioSelectStarNrvNotSelected, ScenarioSelectStar, NotSelected);
};  // namespace NrvScenarioSelectStar

ScenarioSelectStar::ScenarioSelectStar(EffectSystem* pSystem)
    : MultiSceneActor("シナリオ選択のスター", "PowerStar", false), mStarCollectedStatus(0), mStageID(1), mStarIdx(0), mIsPointing(false),
      mRotateSpeed(0.0f), mBasePos(gZeroVec), mTranslationOnSelect(gZeroVec), mAppearFrame(cAppearFrame), mScaleOnSelect(1.0f) {
    initEffect(pSystem, 0, "ScenarioStar");
}

void ScenarioSelectStar::init(const JMapInfoIter& rIter) {
    initNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvNotPointing::sInstance);
}

void ScenarioSelectStar::calcViewAndEntry() {
    if (!isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvAppear::sInstance) || !MultiScene::isLessEqualStep(this, mAppearFrame + 1)) {
        MultiSceneActor::calcViewAndEntry();
    }
}

void ScenarioSelectStar::appear() {
    mScale.set(cNotPointingScale, cNotPointingScale, cNotPointingScale);
    mIsPointing = false;
    mRotateSpeed = 0.0f;
    updatePos();
    MultiScene::startBtp(this, "PowerStar");
    MultiScene::startBva(this, "PowerStar");
    MultiScene::setBtpFrameAndStop(this, PowerStar::getBtpFrameCurrentStage(mStageID));
    MultiScene::setBvaFrameAndStop(this, mStarCollectedStatus == 0 ? 1.0f : 0.0f);
    MultiSceneActor::appear();
    setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvAppear::sInstance);
}

void ScenarioSelectStar::kill() {
    MultiScene::forceDeleteEffectAll(this);
    MultiSceneActor::kill();
}

void ScenarioSelectStar::tryPointing() {
    mIsPointing = true;

    if (isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvNotPointing::sInstance) ||
        isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvEndPointing::sInstance)) {
        setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvPointing::sInstance);
    }
}

void ScenarioSelectStar::select() {
    setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvSelected::sInstance);
}

void ScenarioSelectStar::notSelect() {
    setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvNotSelected::sInstance);
}

bool ScenarioSelectStar::isAppearEnd() const {
    return !_30 && !isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvAppear::sInstance);
}

void ScenarioSelectStar::setup(s32 stageID, int starCollectedStatus, const TVec3f& rPos, s32 starIdx) {
    mStarCollectedStatus = starCollectedStatus;
    mStageID = stageID;
    mBasePos.set(rPos);
    mStarIdx = starIdx;

    mRotation.set< f32 >(0.0f, 0.0f, 150.0f * mStarIdx);
    mAppearFrame = mStarIdx * cAppearDelayFrame + cAppearWaitFrame;
    mRotateTime = mStarIdx * 15 + (mStarIdx % 2 ? 0 : 90);
}

void ScenarioSelectStar::control() {
    mIsPointing = false;
    mRotation.z = MR::repeatDegree(mRotation.z + mRotateSpeed);

    if (isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvNotPointing::sInstance) ||
        isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvPointing::sInstance) ||
        isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvEndPointing::sInstance)) {
        mRotateTime++;
        updatePos();
    }
}

bool ScenarioSelectStar::tryEndPointing() {
    if (!mIsPointing) {
        setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvEndPointing::sInstance);
        return true;
    }

    return false;
}

void ScenarioSelectStar::updatePos() {
    // FIXME: add operation PS misalignment
    // TODO: MR::repeat should probably be MR::repeatDegree?
    // https://decomp.me/scratch/0gALW

    TVec3f offset(JMASinDegree(MR::repeat((mRotateTime * 360.0f) / cMoveInterval, 0.0f, 360.0f)) * cMoveDistance, 0.0f, 0.0f);
    mTranslation.add(mBasePos, offset);
}

void ScenarioSelectStar::exeAppear() {
    if (MultiScene::isStep(this, mAppearFrame)) {
        MultiScene::startBck(this, "ScenarioAppear");
        MR::startSystemSE("SE_OJ_SCEN_SEL_STAR_APPEAR", -1, -1);
    }

    if (MultiScene::isStep(this, mAppearFrame + 1)) {
        if (mStarCollectedStatus == 1) {
            MultiScene::emitEffect(this, "ClearStarLight");
        } else {
            MultiScene::emitEffect(this, "Light");
        }
    }

    if (MultiScene::isGreaterStep(this, mAppearFrame)) {
        MultiScene::setNerveAtBckStopped(this, &NrvScenarioSelectStar::ScenarioSelectStarNrvNotPointing::sInstance);
    }
}

void ScenarioSelectStar::exeNotPointing() {
    if (MultiScene::isFirstStep(this)) {
        mScale.x = cNotPointingScale;
        mScale.y = cNotPointingScale;
        mScale.z = cNotPointingScale;
        mRotateSpeed = cNotPointingRotateSpeedZ;
    }
}

void ScenarioSelectStar::exePointing() {
    if (MultiScene::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_CURSOR_1", -1, -1);
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        mRotateSpeed = cPointingRotateSpeedZ;
    }

    if (MultiScene::isLessStep(this, cStartPointingFrame)) {
        f32 t = MultiScene::calcNerveRate(this, cStartPointingFrame);
        f32 reaction = MR::getScaleWithReactionValueZeroToOne(t, 1.5f, -0.25f);
        f32 linerVal = MR::getLinerValue(reaction, cNotPointingScale, cPointingScale, 1.0f);
        mScale.setAll< f32 >(linerVal);
    } else {
        mScale.x = cPointingScale;
        mScale.y = cPointingScale;
        mScale.z = cPointingScale;
    }

    tryEndPointing();
}

void ScenarioSelectStar::exeEndPointing() {
    if (MultiScene::isFirstStep(this)) {
        mRotateSpeed = cNotPointingRotateSpeedZ;
    }

    mScale.setAll< f32 >(MR::getEaseOutValue(MultiScene::calcNerveRate(this, cEndPointingFrame), cPointingScale, cNotPointingScale, 1.0f));
    MultiScene::setNerveAtStep(this, &NrvScenarioSelectStar::ScenarioSelectStarNrvNotPointing::sInstance, cEndPointingFrame);
}

void ScenarioSelectStar::exeSelected() {
    if (MultiScene::isFirstStep(this)) {
        MultiScene::emitEffect(this, "Select");
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        mTranslationOnSelect.set< f32 >(mTranslation);
        mRotateSpeed = cPointingRotateSpeedZ;
        mScaleOnSelect = mScale.x;
    }

    mScale.setAll< f32 >(MultiScene::calcNerveValue(this, cSelectedWaitFrame, mScaleOnSelect, cPointingScale));
    MultiScene::setNerveAtStep(this, &NrvScenarioSelectStar::ScenarioSelectStarNrvSelectedMove::sInstance, cSelectedWaitFrame);
}

void ScenarioSelectStar::exeSelectedMove() {
    if (MultiScene::isFirstStep(this)) {
        mScale.x = cPointingScale;
        mScale.y = cPointingScale;
        mScale.z = cPointingScale;
    }

    if (MultiScene::isStep(this, cSelectedMoveFrame)) {
        MR::startSystemSE("SE_DM_SENARIO_SEL_ACCEL", -1, -1);
    }

    if (MultiScene::isLessStep(this, cSelectedMoveFrame)) {
        f32 t = MultiScene::calcNerveEaseOutRate(this, cSelectedMoveFrame);
        mTranslation.scale(1.0f - t, mTranslationOnSelect);
        mRotateSpeed = MR::getLinerValue(t, cPointingRotateSpeedZ, cSelectedRotateMoveRate, 1.0f);
    } else {
        mTranslation.z = 0.0f;
        mTranslation.y = 0.0f;
        mTranslation.x = 0.0f;
        mRotateSpeed = MR::getLinerValue(MultiScene::calcNerveRate(this, cSelectedMoveFrame, cSelectedRotateAccelFrame), cSelectedRotateMoveRate,
                                         cSelectedRotateSpeedZ, 1.0f);
    }
}

void ScenarioSelectStar::exeNotSelected() {
    s32 hideDelay = cHideDelayFrame * mStarIdx;

    if (MultiScene::isFirstStep(this)) {
        mRotateSpeed = cNotPointingRotateSpeedZ;
    }

    if (MultiScene::isStep(this, hideDelay)) {
        mRotateSpeed = 0.0f;
        mRotation.z = 150.0f * mStarIdx;
        MultiScene::startBck(this, "ScenarioHide");
        MultiScene::deleteEffectAll(this);
    }

    if (MultiScene::isGreaterStep(this, hideDelay)) {
        if (MultiScene::isBckStopped(this)) {
            kill();
        }
    }
}

ScenarioSelectStar::~ScenarioSelectStar() {
}
