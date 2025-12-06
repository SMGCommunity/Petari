#include "Game/Screen/WipeKoopa.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

namespace {
    NEW_NERVE(WipeKoopaNrvWait, WipeKoopa, Wait);
    NEW_NERVE(WipeKoopaNrvWipeOut, WipeKoopa, WipeOut);
    NEW_NERVE(WipeKoopaNrvClose, WipeKoopa, Close);
};  // namespace

WipeKoopa::WipeKoopa() : WipeLayoutBase("クッパ"), mStepNum(90) {}

void WipeKoopa::init(const JMapInfoIter& rIter) {
    initNerve(&WipeKoopaNrvWait::sInstance);
    initLayoutManager("WipeKoopa", 1);
}

void WipeKoopa::exeWait() {}

void WipeKoopa::exeWipeOut() {
    f32 animRate;

    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "out", 0);

        if (mStepNum > 0) {
            animRate = static_cast< f32 >(MR::getAnimCtrl(this, 0)->mEndFrame) / mStepNum;

            MR::setAnimRate(this, animRate, 0);
        }

        MR::showLayout(this);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&WipeKoopaNrvClose::sInstance);
    }
}

void WipeKoopa::exeClose() {}

void WipeKoopa::wipe(s32 step) {
    setNerve(&WipeKoopaNrvWipeOut::sInstance);
    MR::hideLayout(this);

    if (step > 0) {
        mStepNum = step;
    } else {
        mStepNum = 90;
    }
}

void WipeKoopa::forceClose() {
    setNerve(&WipeKoopaNrvClose::sInstance);
    MR::showLayout(this);
    MR::startAnim(this, "out", 0);
    MR::setAnimFrameAndStop(this, MR::getAnimCtrl(this, 0)->mEndFrame, 0);
}

void WipeKoopa::forceOpen() {
    setNerve(&WipeKoopaNrvWait::sInstance);
    MR::hideLayout(this);
}

bool WipeKoopa::isOpen() const {
    return isNerve(&WipeKoopaNrvWait::sInstance);
}

bool WipeKoopa::isClose() const {
    return isNerve(&WipeKoopaNrvClose::sInstance);
}

bool WipeKoopa::isWipeIn() const {
    return false;
}

bool WipeKoopa::isWipeOut() const {
    return isNerve(&WipeKoopaNrvWipeOut::sInstance);
}
