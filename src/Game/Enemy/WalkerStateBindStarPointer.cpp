#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/Enemy/AnimScaleController.hpp"

namespace {
    static const s32 sPointCanceBindTime = 5;
}

namespace NrvWalkerStateBindStarPointer {
    NEW_NERVE(WalkerStateBindStarPointerNrvBind, WalkerStateBindStarPointer, Bind);
};

WalkerStateBindStarPointer::WalkerStateBindStarPointer(LiveActor* pHost, AnimScaleController* pController)
    : ActorStateBase("歩行型スターポインタ拘束", pHost), mScaleController(pController), mUpdateCounter(0), mHasEffect(false) {
    initNerve(&NrvWalkerStateBindStarPointer::WalkerStateBindStarPointerNrvBind::sInstance);

    if (!MR::isRegisteredEffect(pHost, "Touch")) {
        mHasEffect = true;
        MR::addEffect(pHost, "PointerTouch");
    }
}

void WalkerStateBindStarPointer::appear() {
    mIsDead = false;
    setNerve(&NrvWalkerStateBindStarPointer::WalkerStateBindStarPointerNrvBind::sInstance);
}

void WalkerStateBindStarPointer::kill() {
    mIsDead = true;
    if (mScaleController != nullptr) {
        mScaleController->startAnim();
    }

    MR::deleteEffect(getHost(), mHasEffect ? "PointerTouch" : "Touch");
    if (MR::isExistBck(getHost(), nullptr)) {
        MR::setBckRate(getHost(), 1.0f);
    }
}

bool WalkerStateBindStarPointer::tryStartPointBind() const {
    return MR::isStarPointerPointing2POnPressButton(getHost(), "弱", true, false);
}

void WalkerStateBindStarPointer::exeBind() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        mUpdateCounter = 0;
        MR::emitEffect(getHost(), mHasEffect ? "PointerTouch" : "Touch");
        if (MR::isExistBck(getHost(), nullptr)) {
            MR::setBckRate(getHost(), 0.0f);
        }

        if (mScaleController != nullptr) {
            mScaleController->startDpdHitVibration();
        }
    }

    MR::startDPDFreezeLevelSound(getHost());
    MR::zeroVelocity(getHost());
    if (MR::isStarPointerPointing2POnPressButton(getHost(), "弱", true, false)) {
        mUpdateCounter = 0;
    } else {
        mUpdateCounter++;
    }

    if (mUpdateCounter > sPointCanceBindTime) {
        kill();
    }
}
