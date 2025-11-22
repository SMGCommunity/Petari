#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/PlayTimerScene.hpp"
#include "Game/Screen/TimeLimitLayout.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/ValueControl.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>

namespace {
    static const u32 sTimeUpWaitFrame = 36000;
};

namespace NrvPlayTimerScene {
    NEW_NERVE(PlayTimerSceneNormal, PlayTimerScene, Normal);
    NEW_NERVE(PlayTimerSceneTimeUp, PlayTimerScene, TimeUp);
    NEW_NERVE(PlayTimerSceneFadeoutAfterTimeUp, PlayTimerScene, FadeoutAfterTimeUp);
}; // namespace NrvPlayTimerScene

PlayTimerScene::PlayTimerScene()
    : Scene("PlayTimerScene"),
      mTimeLimitLayout(nullptr),
      mTimeUpLayout(nullptr),
      mTimeUpWaitFrame(sTimeUpWaitFrame),
      _20(nullptr) {
    initNerve(&NrvPlayTimerScene::PlayTimerSceneNormal::sInstance);

    _20 = new ValueControl(30);
    _20->setZero();
}

void PlayTimerScene::init() {
    mTimeLimitLayout = new TimeLimitLayout(sTimeUpWaitFrame);
    mTimeLimitLayout->initWithoutIter();
    mTimeLimitLayout->kill();

    mTimeUpLayout = new TimeUpLayout();
    mTimeUpLayout->initWithoutIter();
    mTimeUpLayout->kill();
}

void PlayTimerScene::start() {
    stop();
    mTimeLimitLayout->appear();
    mTimeLimitLayout->setTimeLimit(mTimeUpWaitFrame);
}

void PlayTimerScene::update() {
    updateNerve();
    _20->update();

    if (isActive()) {
        // FIXME: Should be called via pointer-to-member-function.
        if (mTimeLimitLayout != nullptr) {
            mTimeLimitLayout->movement();
        }

        // FIXME: Should be called via pointer-to-member-function.
        if (mTimeUpLayout != nullptr) {
            mTimeUpLayout->movement();
        }

        // FIXME: Should be called via pointer-to-member-function.
        if (mTimeLimitLayout != nullptr) {
            mTimeLimitLayout->calcAnim();
        }

        // FIXME: Should be called via pointer-to-member-function.
        if (mTimeUpLayout != nullptr) {
            mTimeUpLayout->calcAnim();
        }
    }
}

void PlayTimerScene::draw() const {
    J2DOrthoGraphSimple graph;
    graph.setPort();
    u8               color = MR::lerp(0, 255, _20->getValue());
    JUtility::TColor v1;
    v1.r = color;
    v1.g = color;
    v1.b = color;
    v1.a = color;
    graph.setColor(v1, v1, v1, v1);

    f32    height = JUTVideo::sManager->mRenderModeObj->efbHeight;
    f32    width = MR::getScreenWidth();
    TBox2f box(0.0f, 0.0f, 0.0f + height, 0.0f + width);
    graph.fillBox(box);
}

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
