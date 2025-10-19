#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/PlayTimerScene.hpp"
#include "Game/Screen/TimeLimitLayout.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/ValueControl.hpp"

namespace NrvPlayTimerScene {
    NEW_NERVE(PlayTimerSceneNormal, PlayTimerScene, Normal);
    NEW_NERVE(PlayTimerSceneTimeUp, PlayTimerScene, TimeUp);
    NEW_NERVE(PlayTimerSceneFadeoutAfterTimeUp, PlayTimerScene, FadeoutAfterTimeUp);
};

PlayTimerScene::PlayTimerScene() :
    Scene("PlayTimerScene"),
    mTimeLimitLayout(nullptr),
    mTimeUpLayout(nullptr),
    mTimeLimit(36000),
    _20(nullptr)
{
    initNerve(&NrvPlayTimerScene::PlayTimerSceneNormal::sInstance);

    _20 = new ValueControl(30);
    _20->setZero();
}

void PlayTimerScene::init() {
    mTimeLimitLayout = new TimeLimitLayout(36000);
    mTimeLimitLayout->initWithoutIter();
    mTimeLimitLayout->kill();

    mTimeUpLayout = new TimeUpLayout();
    mTimeUpLayout->initWithoutIter();
    mTimeUpLayout->kill();
}

void PlayTimerScene::start() {
    stop();
    mTimeLimitLayout->appear();
    mTimeLimitLayout->setTimeLimit(mTimeLimit);
}

// PlayTimerScene::update
// PlayTimerScene::draw

bool PlayTimerScene::isActive() const {
    if (mTimeLimitLayout == nullptr) {
        return false;
    }

    return !MR::isDead(mTimeLimitLayout);
}

bool PlayTimerScene::isEndGlobalTimer() const {
    if (!isActive()) {
        return false;
    }

    if (!mTimeLimitLayout->isReadyToTimeUp()) {
        return false;
    }

    return true;
}

void PlayTimerScene::stop() {
    if (mTimeLimitLayout != nullptr) {
        mTimeLimitLayout->kill();
    }

    setNerve(&NrvPlayTimerScene::PlayTimerSceneNormal::sInstance);
}

void PlayTimerScene::startTimeUp() {
    setNerve(&NrvPlayTimerScene::PlayTimerSceneTimeUp::sInstance);
}

void PlayTimerScene::exeNormal() {
    if (MR::isFirstStep(this)) {
        _20->setZero();
    }
}

void PlayTimerScene::exeTimeUp() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_E3_TIMER_TIME_UP", -1, -1);
        mTimeUpLayout->appear();
        MR::startAnim(mTimeUpLayout, "Appear", 0);
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvPlayTimerScene::PlayTimerSceneFadeoutAfterTimeUp::sInstance);
    }
}

void PlayTimerScene::exeFadeoutAfterTimeUp() {
    if (MR::isFirstStep(this)) {
        _20->setDirToOneResetFrame();
    }

    if (_20->mFrame == _20->mMaxFrame) {
        mTimeLimitLayout->kill();
        mTimeUpLayout->kill();
        setNerve(&NrvPlayTimerScene::PlayTimerSceneNormal::sInstance);
        MR::requestChangeSceneAfterBoot();
    }
}
