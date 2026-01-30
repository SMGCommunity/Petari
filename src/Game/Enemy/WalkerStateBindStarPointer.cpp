#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/Enemy/AnimScaleController.hpp"

namespace NrvWalkerStateBindStarPointer {
    NEW_NERVE(WalkerStateBindStarPointerNrvBind, WalkerStateBindStarPointer, Bind);
};

WalkerStateBindStarPointer::WalkerStateBindStarPointer(LiveActor* pActor, AnimScaleController* pController)
    : ActorStateBase("歩行型スターポインタ拘束", pActor), mScaleController(pController), mUpdateCounter(0), mHasEffect(false) {
    initNerve(&NrvWalkerStateBindStarPointer::WalkerStateBindStarPointerNrvBind::sInstance);

    if (!MR::isRegisteredEffect(pActor, "Touch")) {
        mHasEffect = true;
        MR::addEffect(pActor, "PointerTouch");
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

    const char* effectName = mHasEffect ? "PointerTouch" : "Touch";
    MR::deleteEffect(mHost, effectName);
    if (MR::isExistBck(mHost, nullptr)) {
        MR::setBckRate(mHost, 1.0f);
    }
}

bool WalkerStateBindStarPointer::tryStartPointBind() const {
    return MR::isStarPointerPointing2POnPressButton(mHost, "弱", true, false);
}

void WalkerStateBindStarPointer::exeBind() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        mUpdateCounter = 0;
        const char* effectName = mHasEffect ? "PointerTouch" : "Touch";
        MR::emitEffect(mHost, effectName);
        if (MR::isExistBck(mHost, nullptr)) {
            MR::setBckRate(mHost, 0.0f);
        }

        if (mScaleController != nullptr) {
            mScaleController->startDpdHitVibration();
        }
    }

    MR::startDPDFreezeLevelSound(mHost);
    MR::zeroVelocity(mHost);
    if (MR::isStarPointerPointing2POnPressButton(mHost, "弱", true, false)) {
        mUpdateCounter = 0;
    } else {
        mUpdateCounter++;
    }

    if (mUpdateCounter > 5) {
        update();
    }
}

WalkerStateBindStarPointer::~WalkerStateBindStarPointer() {}
