#include "Game/Screen/TimeLimitLayout.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/ValueControl.hpp"

void TimeLimitLayout_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    static const Timing sTimingTable[] = {
        {18000, 100, true, true, true}, {10800, 100, true, true, true},  {3600, 100, true, true, true},
        {1800, 100, true, true, false}, {600, 600, false, false, false},
    };
    static const s32 sScaleUpDownFrame = 8;
    static const f32 sScaleMaxAdd = 0.5f;
    static const s32 sFadeoutBeforeFrame = 90;
    static const s32 sFadeinoutFrame = 60;
}  // namespace

void TimeUpLayout::init(const JMapInfoIter& rIter) {
    initLayoutManager("TimeUp", 1);
}

namespace NrvTimeLimitLayout {
    NEW_NERVE(TimeLimitLayoutAppear, TimeLimitLayout, Appear);
    NEW_NERVE(TimeLimitLayoutCountDown, TimeLimitLayout, CountDown);
    NEW_NERVE(TimeLimitLayoutScaleUp, TimeLimitLayout, ScaleUp);
    NEW_NERVE(TimeLimitLayoutScaleKeep, TimeLimitLayout, ScaleKeep);
    NEW_NERVE(TimeLimitLayoutScaleDown, TimeLimitLayout, ScaleDown);
    NEW_NERVE(TimeLimitLayoutFadeout, TimeLimitLayout, Fadeout);
    NEW_NERVE(TimeLimitLayoutTimeUpReady, TimeLimitLayout, TimeUpReady);
}  // namespace NrvTimeLimitLayout

TimeLimitLayout::TimeLimitLayout(u32 timeLimit)
    : LayoutActor("タイムリミット", true), mTime(), mTimeLimit(timeLimit), mScaleControl(), mFadeControl(), mCurrentTiming(), mIsSuspend(), _35() {
}

void TimeLimitLayout::init(const JMapInfoIter& rIter) {
    initLayoutManager("TimeLimit", 1);

    mScaleControl = new ValueControl(::sScaleUpDownFrame);
    mScaleControl->setZero();

    mFadeControl = new ValueControl(::sFadeinoutFrame);
    mFadeControl->setOne();

    MR::setInfluencedAlphaToChild(this);
    initNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutCountDown));
    kill();
}

void TimeLimitLayout::appear() {
    resetFrame();
    LayoutActor::appear();
    setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutAppear));
}

void TimeLimitLayout::kill() {
    LayoutActor::kill();
}

void TimeLimitLayout::setTimeLimit(u32 timeLimit) {
    mTimeLimit = timeLimit;
}

void TimeLimitLayout::setDisplayModeOnNormal(bool param1) {
    _35 = param1;
}

bool TimeLimitLayout::isReadyToTimeUp() const {
    return !MR::isDead(this) && isNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutTimeUpReady));
}

void TimeLimitLayout::resetFrame() {
    mTime = 0;

    updateTextBox();

    mIsSuspend = false;

    mScaleControl->setOne();
    mFadeControl->setOne();

    mCurrentTiming = &::sTimingTable[0];
}

namespace {
    inline bool equalTime(const u32& rTime, const u32& rLimit) {
        return rTime == rLimit;
    }
}  // namespace

void TimeLimitLayout::addFrame() {
    if (equalTime(mTime, mTimeLimit)) {
        return;
    }

    mTime++;

    updateTextBox();
}

void TimeLimitLayout::updateTextBox() {
    MR::updateMinuteAndSecondTextBox(this, "Number", (mTimeLimit - mTime) + 59);
}

void TimeLimitLayout::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_E3_TIMER_SCALE_UP");
        MR::startAnim(this, "Appear", 0);
    }

    MR::setNerveAtAnimStopped(this, GET_NERVE(TimeLimitLayout, TimeLimitLayoutCountDown), 0);
}

void TimeLimitLayout::exeCountDown() {
    if (updateNormal()) {
        return;
    }

    const Timing* pTiming = getCurrentTiming();

    if (pTiming != nullptr) {
        mCurrentTiming = pTiming;

        MR::startSystemSE("SE_SY_E3_TIMER_SCALE_UP");
        setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutScaleUp));
    }
}

void TimeLimitLayout::exeScaleUp() {
    if (MR::isFirstStep(this)) {
        mScaleControl->setDirToOneResetFrame();
        mFadeControl->setOne();
    }

    if (updateNormal()) {
        return;
    }

    if (mScaleControl->mFrame == mScaleControl->mMaxFrame) {
        setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutScaleKeep));
    }
}

void TimeLimitLayout::exeScaleKeep() {
    if (MR::isFirstStep(this)) {
        if (mCurrentTiming->mIsScaleUp) {
            MR::startAnim(this, "TimeSignal", 0);
        }
    }

    if (updateNormal()) {
        return;
    }

    if (MR::isGreaterStep(this, mCurrentTiming->mScaleKeepFrame)) {
        if (mCurrentTiming->mIsScaleDown) {
            setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutScaleDown));
        } else {
            setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutCountDown));
        }
    }
}

void TimeLimitLayout::exeScaleDown() {
    if (MR::isFirstStep(this)) {
        mScaleControl->setDirToZeroResetFrame();
    }

    if (updateNormal()) {
        return;
    }

    if (mScaleControl->mFrame == mScaleControl->mMaxFrame) {
        if (mCurrentTiming->_A && !_35) {
            setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutFadeout));
        } else {
            setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutCountDown));
        }
    }
}

void TimeLimitLayout::exeFadeout() {
    if (MR::isStep(this, ::sFadeoutBeforeFrame)) {
        mFadeControl->setDirToZeroResetFrame();
    }

    if (updateNormal()) {
        return;
    }

    if (mFadeControl->mFrame == 0) {
        setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutCountDown));
    }
}

void TimeLimitLayout::exeTimeUpReady() {
}

void TimeLimitLayout::control() {
    mFadeControl->update();
    mScaleControl->update();

    f32 paneScale = 1.0f + MR::getEaseInOutValue(mScaleControl->getValue(), 0.0f, 1.0f, 1.0f) * ::sScaleMaxAdd;

    MR::setPaneScale(this, paneScale, paneScale, "TimeLimit");
    MR::setLayoutAlphaFloat(this, MR::getEaseInOutValue(mFadeControl->getValue(), 0.0f, 1.0f, 1.0f));
}

const Timing* TimeLimitLayout::getCurrentTiming() const {
    for (s32 i = 0; i < ARRAY_SIZE(::sTimingTable); i++) {
        u32 scaleStartFrame = ::sTimingTable[i].mScaleStartFrame;
        u32 timeLimit = mTimeLimit;
        u32 frame = scaleStartFrame - ::sTimingTable[i].mScaleKeepFrame;

        if (timeLimit >= frame && scaleStartFrame >= timeLimit) {
            continue;
        }

        u32 timeLeft = timeLimit - mTime;

        if (frame > timeLeft || timeLeft >= scaleStartFrame) {
            continue;
        }

        return &::sTimingTable[i];
    }

    return nullptr;
}

bool TimeLimitLayout::updateNormal() {
    if (mIsSuspend) {
        return false;
    }

    addFrame();

    u32 timeLeft = mTimeLimit - mTime;

    if (timeLeft <= 120) {
        if (timeLeft % 60 == 0 && timeLeft != 0) {
            MR::startSystemSE("SE_SY_E3_TIMER_COUNT_4");
        }
    } else if (timeLeft <= 360) {
        if (timeLeft % 60 == 0) {
            MR::startSystemSE("SE_SY_E3_TIMER_COUNT_2");
        }
    } else if (timeLeft <= 600) {
        if (timeLeft % 60 == 0) {
            MR::startSystemSE("SE_SY_E3_TIMER_COUNT_1");
        }
    }

    if (mTimeLimit - mTime == 600) {
        MR::startAnim(this, "TimeLimit", 0);
    }

    if (mTime == mTimeLimit) {
        setNerve(GET_NERVE(TimeLimitLayout, TimeLimitLayoutTimeUpReady));

        return true;
    }

    return false;
}
