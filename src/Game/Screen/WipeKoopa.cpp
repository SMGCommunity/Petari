#include "Game/Screen/WipeKoopa.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

namespace {
    static const s32 sAnimFrame = 90;
};  // namespace

namespace {
    NEW_NERVE(WipeKoopaNrvWait, WipeKoopa, Wait);
    NEW_NERVE(WipeKoopaNrvWipeOut, WipeKoopa, WipeOut);
    NEW_NERVE(WipeKoopaNrvClose, WipeKoopa, Close);
};  // namespace

WipeKoopa::WipeKoopa() : WipeLayoutBase("クッパ"), mFrame(::sAnimFrame) {
}

void WipeKoopa::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE_GLOBAL(WipeKoopaNrvWait));
    initLayoutManager("WipeKoopa", 1);
}

void WipeKoopa::exeWait() {
}

void WipeKoopa::exeWipeOut() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "out", 0);

        if (mFrame > 0) {
            f32 animRate = static_cast< f32 >(MR::getAnimCtrl(this, 0)->getEnd()) / mFrame;

            MR::setAnimRate(this, animRate, 0);
        }

        MR::showLayout(this);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE_GLOBAL(WipeKoopaNrvClose));
    }
}

void WipeKoopa::exeClose() {
}

void WipeKoopa::wipe(s32 frame) {
    setNerve(GET_NERVE_GLOBAL(WipeKoopaNrvWipeOut));
    MR::hideLayout(this);

    if (frame > 0) {
        mFrame = frame;
    } else {
        mFrame = ::sAnimFrame;
    }
}

void WipeKoopa::forceClose() {
    setNerve(GET_NERVE_GLOBAL(WipeKoopaNrvClose));
    MR::showLayout(this);
    MR::startAnim(this, "out", 0);
    MR::setAnimFrameAndStop(this, MR::getAnimCtrl(this, 0)->getEnd(), 0);
}

void WipeKoopa::forceOpen() {
    setNerve(GET_NERVE_GLOBAL(WipeKoopaNrvWait));
    MR::hideLayout(this);
}

bool WipeKoopa::isOpen() const {
    return isNerve(GET_NERVE_GLOBAL(WipeKoopaNrvWait));
}

bool WipeKoopa::isClose() const {
    return isNerve(GET_NERVE_GLOBAL(WipeKoopaNrvClose));
}

bool WipeKoopa::isWipeIn() const {
    return false;
}

bool WipeKoopa::isWipeOut() const {
    return isNerve(GET_NERVE_GLOBAL(WipeKoopaNrvWipeOut));
}
