#include "Game/MapObj/SuperSpinDriver.h"

/* it seems like this file was compiled with an earlier compiler version */

bool SuperSpinDriver::tryEndCapture() {
    if (MR::isGreaterStep(this, 60) && PSVECDistance(_C4.toCVec(), mPosition.toCVec()) < 15.0f) {
        cancelBind();
        _174 = 0;
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvWait::sInstance);
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryForceCancel() {
    if (_8C == NULL) {
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvCoolDown::sInstance);
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryShootStart() {
    bool flag = false;

    if (MR::isPadSwing(0) || MR::isPlayerPointedBy2POnTriggerButton()) {
        flag = true;
    }

    if (flag) {
        MR::deleteEffect(this, "SuperSpinDriverLight");
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvShootStart::sInstance);
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryShoot() {
    if (MR::isGreaterStep(this, 45)) {
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvShoot::sInstance);
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryEndShoot() {
    if (MR::isGreaterEqualStep(this, _150)) {
        endBind();
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvCoolDown::sInstance);
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryEndCoolDown() {
    if (MR::isGreaterStep(this, 60) && _178) {
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvWait::sInstance);
        return true;
    }

    return false;
}

bool SuperSpinDriver::trySwitchOff() {
    if (MR::isValidSwitchAppear(this) && !MR::isOnSwitchAppear(this)) {
        kill();
        return true;
    }

    return false;
}

void SuperSpinDriver::requestAppear() {
    MR::invalidateClipping(this);

    if (mSpinDriverCamera->isUseAppearCamera(this)) {
        MR::requestStartDemo(this, "出現", &NrvSuperSpinDriver::SuperSpinDriverNrvAppear::sInstance, &NrvSuperSpinDriver::SuperSpinDriverNrvTryDemo::sInstance);
    }
    else {
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvAppear::sInstance);
    }
}

void SuperSpinDriver::requestEmptyAppear() {
    MR::invalidateClipping(this);

    if (mSpinDriverCamera->isUseAppearCamera(this)) {
        MR::requestStartDemo(this, "出現", &NrvSuperSpinDriver::SuperSpinDriverNrvEmptyAppear::sInstance, &NrvSuperSpinDriver::SuperSpinDriverNrvTryDemo::sInstance);
    }
    else {
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvEmptyAppear::sInstance);
    }
}

void SuperSpinDriver::requestActive() {
    if (isNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvNonActive::sInstance)) {
        requestAppear();
    }
    else if (isNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvEmptyNonActive::sInstance)) {
        requestEmptyAppear();
    }
}

void SuperSpinDriver::requestHide() {
    if (!MR::isDead(this)) {
        if (_8C != NULL) {
            endBind();
        }

        makeActorDead();
    }
}

void SuperSpinDriver::requestShow() {
    if (MR::isDead(this)) {
        makeActorAppeared();
    }
}

void SuperSpinDriver::exeEmptyNonActive() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    if (isRightToUse()) {
        onUse();
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvNonActive::sInstance);
    }
}

void SuperSpinDriver::exeEmptyAppear() {
    if (MR::isFirstStep(this)) {
        mSpinDriverCamera->startAppearCamera(this, _100, _E8, mPosition);

        if (!_17F) {
            MR::startSystemSE("SE_SY_SPIN_DRIVER_APPEAR", -1, -1);
            MR::startSound(this, "SE_OJ_S_SPIN_DRV_APPEAR", -1, -1);
        }
    }

    if (MR::isBckStopped(this)) {
        s32 frames = mSpinDriverCamera->getAppearCameraFrames();

        if (MR::isGreaterStep(this, frames)) {
            setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvEmptyWait::sInstance);

            if (mSpinDriverCamera->isUseAppearCamera(this)) {
                mSpinDriverCamera->endAppearCamera(this);
                MR::endDemoWaitCameraInterpolating(this, "出現");
            }
        }
    }
}

void SuperSpinDriver::exeEmptyWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    if (isRightToUse()) {
        onUse();
        setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvWait::sInstance);
    }
}

void SuperSpinDriver::exeNonActive() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "NonActive", NULL);
        MR::validateClipping(this);
    }

    addSwingSignRotateY();
}

void SuperSpinDriver::exeAppear() {
    if (MR::isFirstStep(this)) {
        mSpinDriverCamera->startAppearCamera(this, _100, _E8, mPosition);

        if (!_17F) {
            MR::startSystemSE("SE_SY_SPIN_DRIVER_APPEAR", -1, -1);
            MR::startSound(this, "SE_OJ_S_SPIN_DRV_APPEAR", -1, -1);
        }

        MR::startBck(this, "Appear", NULL);
        _144 = 0.0f;
    }

    if (MR::isBckStopped(this)) {
        s32 frames = mSpinDriverCamera->getAppearCameraFrames();

        if (MR::isGreaterStep(this, frames)) {
            setNerve(&NrvSuperSpinDriver::SuperSpinDriverNrvWait::sInstance);

            if (mSpinDriverCamera->isUseAppearCamera(this)) {
                mSpinDriverCamera->endAppearCamera(this);
                MR::endDemoWaitCameraInterpolating(this, "出現");
            }
        }
    }
}

void SuperSpinDriver::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", NULL);
        MR::validateClipping(this);
    }

    if (MR::isGreaterStep(this, sCanBindTime)) {
        addSwingSignRotateY();
    }

    if (_178 > 0) {
        MR::startLevelSound(this, "SE_OJ_LV_S_SPIN_DRV_SHINE", -1, -1, -1);

        if (!_17C) {
            MR::emitEffect(this, "SuperSpinDriverLight");
            MR::startCSSound("CS_SPIN_BIND", NULL, 0);
        }
        
    }
    else {
        MR::deleteEffect(this, "SuperSpinDriverLight");
    }

    trySwitchOff();
}

void SuperSpinDriver::exeCapture() {
    if (tryForceCancel()) {
        MR::deleteEffect(this, "SuperSpinDriverLight");
    }
    else {
        if (MR::isFirstStep(this)) {
            MR::emitEffect(this, "SuperSpinDriverLight");
            MR::startBckPlayer("SpinDriverWait", "SuperSpinDriverCapture");
            _144 = 0.0f;
        }

        MR::startLevelSound(this, "SE_OJ_LV_S_SPIN_DRV_SHINE", -1, -1, -1);
        MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_CAPTURE", -1, -1, -1);
        moveBindPosToCenter();
        f32 rate = MR::calcNerveRate(this, 60);
        _134 = rate;
        updateBindActorPoseToShoot((f64)rate);
        _144 += 0.0040000002f;
        MR::tryRumblePadWeak(this, 0);
        _178 = 60;

        if (!tryEndCapture()) {
            if (!tryShootStart()) {
                return;
            }
        }
    }
}

void SuperSpinDriver::updateShootMotion() {
    if (MR::isStep(this, _154)) {
        MR::startBckPlayer("SpaceFlyLoop", "SuperSpinDriverFlyLoop");
    }

    if (MR::isLessStep(this, _158)) {
        MR::startLevelSound(_8C, "SE_PM_LV_S_SPIN_DRV_FLY", -1, -1, -1);
    }

    if (MR::isStep(this, _158)) {
        MR::startBckPlayer("SpaceFlyEnd", "SuperSpinDriverFlyEnd");
        MR::startSound(_8C, "SE_PM_S_SPIN_DRV_COOL_DOWN", -1, -1);
        MR::startSound(_8C, "SE_PV_JUMP_S", -1, -1);
    }

    if (MR::isStep(this, _15C)) {
        MR::startBckPlayer("Fall", "SuperSpinDriverFall");
    }
}

void SuperSpinDriver::cancelBind() {
    if (_8C != NULL) {
        MR::endBindAndPlayerJump(this, _D0, 0);
        _8C = NULL;
    }

    mSpinDriverCamera->cancel();
}

void SuperSpinDriver::endBind() {
    MR::endBindAndSpinDriverJump(this, _D0);
    _8C = NULL;
    mSpinDriverCamera->end();
}

void SuperSpinDriver::startPathDraw() {
    if (mPathDrawer != NULL) {
        if (MR::isDead(mPathDrawer)) {
            mPathDrawer->appear();
            MR::emitEffect(this, "EndGlow");
        }
    }
}

void SuperSpinDriver::endPathDraw() {
    if (mPathDrawer != NULL) {
        if (!MR::isDead(mPathDrawer)) {
            MR::emitEffect(this, "EndGlow");
        }
    }
}

/*
void SuperSpinDriver::calcShootMotionTime() {
    if (_150 >= 20) {
        if (_150 >= 70) {
            _158 = _150 - 70;
            _15C = _150 - 20;

            f32 v2 = 0.2f * _150;

            if (v2 > 90) {
                v2 = 90;
            }

            _154 = 10 * (v2 / 10);
        }
    }
}
*/

void SuperSpinDriver::addSwingSignRotateY() {
    bool flag = false;

    if (MR::isPadSwing(0) || MR::isPlayerPointedBy2POnTriggerButton()) {
        flag = true;
    }

    if (flag) {
        f32 v4 = MR::add(_144, 0.1f);
        _144 += 0.1f;

        if (v4 > 0.23f) {
            _144 = 0.23f;
        }
    }
}

void SuperSpinDriver::onUse() {
    if (mEmptyModel != NULL) {
        mEmptyModel->kill();
    }

    MR::showModel(this);
}

void SuperSpinDriver::offUse() {
    if (mEmptyModel != NULL) {
        mEmptyModel->appear();
    }

    MR::hideModelAndOnCalcAnim(this);
}

/*
bool SuperSpinDriver::isRightToUse() const {
    //return (mColor == 1) ? true : MR::isOnGameEventFlagGreenDriver();
}
*/

void SuperSpinDriver::exeCoolDown() {
    // BUG, is supposed to be a conditional to call tryEndCoolDown
    MR::isFirstStep(this);
    if (!tryEndCoolDown()) {
        trySwitchOff();
    }
}

void SuperSpinDriver::exeShootOnEnd() {
    MR::invalidateHitSensor(this, "body");
    mOperateRing->reset();
}

namespace MR {
    NameObj* createSuperSpinDriverYellow(const char *pName) {
        return new SuperSpinDriver(pName, 0);
    }

    NameObj* createSuperSpinDriverGreen(const char *pName) {
        return new SuperSpinDriver(pName, 1);
    }

    NameObj* createSuperSpinDriverPink(const char *pName) {
        return new SuperSpinDriver(pName, 2);
    }
};

namespace NrvSuperSpinDriver {
    INIT_NERVE(SuperSpinDriverNrvTryDemo);
    INIT_NERVE(SuperSpinDriverNrvEmptyNonActive);
    INIT_NERVE(SuperSpinDriverNrvEmptyAppear);
    INIT_NERVE(SuperSpinDriverNrvEmptyWait);
    INIT_NERVE(SuperSpinDriverNrvNonActive);
    INIT_NERVE(SuperSpinDriverNrvAppear);
    INIT_NERVE(SuperSpinDriverNrvWait);
    INIT_NERVE(SuperSpinDriverNrvCapture);
    INIT_NERVE(SuperSpinDriverNrvShootStart);
    INIT_NERVE(SuperSpinDriverNrvShoot);
    INIT_NERVE(SuperSpinDriverNrvCoolDown);
};