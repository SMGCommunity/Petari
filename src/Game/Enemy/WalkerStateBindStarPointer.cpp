#include "Game/Enemy/WalkerStateBindStarPointer.hpp"

WalkerStateBindStarPointer::WalkerStateBindStarPointer(LiveActor *pActor, AnimScaleController *pController) : ActorStateBase("歩行型スターポインタ拘束") {
    mHostActor = pActor;
    mScaleController = pController;
    mUpdateCounter = 0;
    mHasEffect = false;
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
    MR::deleteEffect(mHostActor, effectName);
    if (MR::isExistBck(mHostActor, nullptr)) {
        MR::setBckRate(mHostActor, 1.0f);
    }
}

bool WalkerStateBindStarPointer::tryStartPointBind() const {
    return MR::isStarPointerPointing2POnPressButton(mHostActor, "弱", true, false);
}

void WalkerStateBindStarPointer::exeBind() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        mUpdateCounter = 0;
        const char* effectName = mHasEffect ? "PointerTouch" : "Touch";
        MR::emitEffect(mHostActor, effectName);
        if (MR::isExistBck(mHostActor, nullptr)) {
            MR::setBckRate(mHostActor, 0.0f);
        }

        if (mScaleController != nullptr) {
            mScaleController->startDpdHitVibration();
        }
    }

    MR::startDPDFreezeLevelSound(mHostActor);
    MR::zeroVelocity(mHostActor);
    if (MR::isStarPointerPointing2POnPressButton(mHostActor, "弱", true, false)) {
        mUpdateCounter = 0;
    }
    else {
        mUpdateCounter++;
    }

    if (mUpdateCounter > 5) {
        update();
    }
}

WalkerStateBindStarPointer::~WalkerStateBindStarPointer() {

}

namespace NrvWalkerStateBindStarPointer {
    INIT_NERVE(WalkerStateBindStarPointerNrvBind);
};