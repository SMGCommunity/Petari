#include "Game/MapObj/FlameGun.hpp"

FlameGun::FlameGun(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = -1.0f;
    _8C.z = 0.0f;
    mState = -1;
}

void FlameGun::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::setGroupClipping(this, rIter, 32);
    s32 arg = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    mState = arg;

    if (MR::useStageSwitchReadA(this, rIter)) {
        initNerve(&NrvFlameGun::HostTypeNrvSwitchWait::sInstance);
    }
    else {
        switch (mState) {
            case 0:
                initNerve(&NrvFlameGun::HostTypeNrvSwitchWait::sInstance);
                break;
            case 1:
                initNerve(&NrvFlameGun::HostTypeNrvRotate::sInstance);
                break;
            case 2:
                initNerve(&NrvFlameGun::HostTypeNrvRadiateOnly::sInstance);
                break;
        }
    }

    initModelManagerWithAnm("FlameGun", nullptr, false);
    MR::connectToSceneEnemy(this);
    initSound(4, false);
    initBinder((77.0f * mScale.y), (77.0f * mScale.y), 0);
    initEffectKeeper(3, nullptr, false);
    initSensor();
    MR::initShadowVolumeSphere(this, 77.0f);
    appear();
    MR::invalidateHitSensor(this, "attack");
}

void FlameGun::initSensor() {
    f32 scale = mScale.y;
    initHitSensor(2);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 77.0f * scale;
    offs.z = 0.0f;
    MR::addHitSensorEnemy(this, "body", 0x20, offs.y, offs);
    MR::addHitSensorCallbackEnemy(this, "attack", 0x10, (80.0f * scale));
}

void FlameGun::initAfterPlacement() {
    MR::offCalcShadow(this, nullptr);
}

void FlameGun::endClipped() {
    LiveActor::endClipped();

    if (!isNerve(&NrvFlameGun::HostTypeNrvRadiate::sInstance) && !isNerve(&NrvFlameGun::HostTypeNrvRotate::sInstance)) {
        isNerve(&NrvFlameGun::HostTypeNrvRadiateOnly::sInstance);
    }
}

void FlameGun::exeSwitchWait() {
    if (MR::isFirstStep(this)) {
        MR::forceDeleteEffectAll(this);
    }

    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        switch (mState) {
            case 0:
                setNerve(&NrvFlameGun::HostTypeNrvPreRadiate::sInstance);
                break;
            case 1:
                setNerve(&NrvFlameGun::HostTypeNrvRotate::sInstance);
                break;
            case 2:
                setNerve(&NrvFlameGun::HostTypeNrvRadiateOnly::sInstance);
                break;
        }
    }
}

void FlameGun::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvFlameGun::HostTypeNrvPreRadiate::sInstance);
    }
}

void FlameGun::exePreRadiate() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShootingBefore", nullptr);
        MR::startBrk(this, "ShootingBefore");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvFlameGun::HostTypeNrvRadiate::sInstance);
        MR::setBrkFrameAndStop(this, 0.0f);
    }
}

void FlameGun::exeRadiate() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shooting", nullptr);
        MR::validateHitSensor(this, "attack");
    }

    MR::startLevelSound(this, "SE_EM_LV_FLAMEGUN_FIRE", -1, -1, -1);

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvFlameGun::HostTypeNrvWait::sInstance);
        MR::forceDeleteEffectAll(this);
        MR::invalidateHitSensor(this, "attack");
    }
}

void FlameGun::exeRotate() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shooting", nullptr);
        MR::validateHitSensor(this, "attack");
    }

    mRotation.y += 0.89999998f;
    MR::startLevelSound(this, "SE_EM_LV_FLAMEGUN_FIRE", -1, -1, -1);

    if (!MR::isValidHitSensor(this, "attack")) {
        MR::validateHitSensor(this, "attack");
    }
}

void FlameGun::exeRadiateOnly() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shooting", nullptr);
        MR::validateHitSensor(this, "attack");
    }

    MR::startLevelSound(this, "SE_EM_LV_FLAMEGUN_FIRE", -1, -1, -1);
}

// FlameGun::updateHitSensor

void FlameGun::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (!MR::sendMsgEnemyAttackFire(a2, a1)) {
        MR::sendMsgPush(a2, a1);
    }
}

FlameGun::~FlameGun() {

}

namespace NrvFlameGun {
    INIT_NERVE(HostTypeNrvSwitchWait);
    INIT_NERVE(HostTypeNrvWait);
    INIT_NERVE(HostTypeNrvPreRadiate);
    INIT_NERVE(HostTypeNrvRadiate);
    INIT_NERVE(HostTypeNrvRotate);
    INIT_NERVE(HostTypeNrvRadiateOnly);
};