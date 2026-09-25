#include "Game/Scene/PlayTimerScene.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/TimeLimitLayout.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/ValueControl.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>

namespace {
    static const u32 sTimeUpWaitFrame = 36000;
};  // namespace

namespace NrvPlayTimerScene {
    NEW_NERVE(PlayTimerSceneNormal, PlayTimerScene, Normal);
    NEW_NERVE(PlayTimerSceneTimeUp, PlayTimerScene, TimeUp);
    NEW_NERVE(PlayTimerSceneFadeoutAfterTimeUp, PlayTimerScene, FadeoutAfterTimeUp);
};  // namespace NrvPlayTimerScene

PlayTimerScene::PlayTimerScene() : Scene("PlayTimerScene"), mTimeLimitLayout(), mTimeUpLayout(), mTimeUpWaitFrame(::sTimeUpWaitFrame), _20() {
    initNerve(GET_NERVE(PlayTimerScene, PlayTimerSceneNormal));

    _20 = new ValueControl(30);
    _20->setZero();
}

void PlayTimerScene::init() {
    mTimeLimitLayout = new TimeLimitLayout(::sTimeUpWaitFrame);
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
        {
            void (LayoutActor::*pMethod)() = &LayoutActor::movement;
            if (mTimeLimitLayout != nullptr) {
                (mTimeLimitLayout->*pMethod)();
            }
        }

        {
            void (LayoutActor::*pMethod)() = &LayoutActor::movement;
            if (mTimeUpLayout != nullptr) {
                (mTimeUpLayout->*pMethod)();
            }
        }

        {
            void (LayoutActor::*pMethod)() = &LayoutActor::calcAnim;
            if (mTimeLimitLayout != nullptr) {
                (mTimeLimitLayout->*pMethod)();
            }
        }

        {
            void (LayoutActor::*pMethod)() = &LayoutActor::calcAnim;
            if (mTimeUpLayout != nullptr) {
                (mTimeUpLayout->*pMethod)();
            }
        }
    }
}

void PlayTimerScene::draw() const {
    if (mTimeLimitLayout != nullptr) {
        mTimeLimitLayout->draw();
    }

    if (mTimeUpLayout != nullptr) {
        mTimeUpLayout->draw();
    }

    if (_20->mFrame == 0) {
        return;
    }

    J2DOrthoGraphSimple graph;
    graph.setPort();
    u8 alpha = MR::lerp(0, 255, _20->getValue());
    JUtility::TColor color(0, 0, 0, alpha);
    graph.setColor(color);

    f32 height = static_cast< s32 >(JUTVideo::getManager()->getRenderMode()->efbHeight);
    f32 width = MR::getScreenWidth();
    graph.fillBox(0.0f, 0.0f, width, height);
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

    setNerve(GET_NERVE(PlayTimerScene, PlayTimerSceneNormal));
}

void PlayTimerScene::startTimeUp() {
    setNerve(GET_NERVE(PlayTimerScene, PlayTimerSceneTimeUp));
}

void PlayTimerScene::exeNormal() {
    if (MR::isFirstStep(this)) {
        _20->setZero();
    }
}

void PlayTimerScene::exeTimeUp() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_E3_TIMER_TIME_UP");
        mTimeUpLayout->appear();
        MR::startAnim(mTimeUpLayout, "Appear", 0);
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(GET_NERVE(PlayTimerScene, PlayTimerSceneFadeoutAfterTimeUp));
    }
}

void PlayTimerScene::exeFadeoutAfterTimeUp() {
    if (MR::isFirstStep(this)) {
        _20->setDirToOneResetFrame();
    }

    if (_20->mFrame == _20->mMaxFrame) {
        mTimeLimitLayout->kill();
        mTimeUpLayout->kill();
        setNerve(GET_NERVE(PlayTimerScene, PlayTimerSceneNormal));
        MR::requestChangeSceneAfterBoot();
    }
}
