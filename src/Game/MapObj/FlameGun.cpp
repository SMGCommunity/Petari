#include "Game/MapObj/FlameGun.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvFlameGun {
    NEW_NERVE(HostTypeNrvSwitchWait, FlameGun, SwitchWait);
    NEW_NERVE(HostTypeNrvWait, FlameGun, Wait);
    NEW_NERVE(HostTypeNrvPreRadiate, FlameGun, PreRadiate);
    NEW_NERVE(HostTypeNrvRadiate, FlameGun, Radiate);
    NEW_NERVE(HostTypeNrvRotate, FlameGun, Rotate);
    NEW_NERVE(HostTypeNrvRadiateOnly, FlameGun, RadiateOnly);
};  // namespace NrvFlameGun

FlameGun::FlameGun(const char* pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = -1.0f;
    _8C.z = 0.0f;
    mState = -1;
}

void FlameGun::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::setGroupClipping(this, rIter, 32);
    s32 arg = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    mState = arg;

    if (MR::useStageSwitchReadA(this, rIter)) {
        initNerve(GET_NERVE(FlameGun, HostTypeNrvSwitchWait));
    } else {
        switch (mState) {
        case 0:
            initNerve(GET_NERVE(FlameGun, HostTypeNrvSwitchWait));
            break;
        case 1:
            initNerve(GET_NERVE(FlameGun, HostTypeNrvRotate));
            break;
        case 2:
            initNerve(GET_NERVE(FlameGun, HostTypeNrvRadiateOnly));
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
    MR::addHitSensorEnemy(this, "body", 32, offs.y, offs);
    MR::addHitSensorCallbackEnemy(this, "attack", 16, (80.0f * scale));
}

void FlameGun::initAfterPlacement() {
    MR::offCalcShadow(this, nullptr);
}

void FlameGun::endClipped() {
    LiveActor::endClipped();

    if (!isNerve(GET_NERVE(FlameGun, HostTypeNrvRadiate)) && !isNerve(GET_NERVE(FlameGun, HostTypeNrvRotate))) {
        isNerve(GET_NERVE(FlameGun, HostTypeNrvRadiateOnly));
    }
}

void FlameGun::exeSwitchWait() {
    if (MR::isFirstStep(this)) {
        MR::forceDeleteEffectAll(this);
    }

    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        switch (mState) {
        case 0:
            setNerve(GET_NERVE(FlameGun, HostTypeNrvPreRadiate));
            break;
        case 1:
            setNerve(GET_NERVE(FlameGun, HostTypeNrvRotate));
            break;
        case 2:
            setNerve(GET_NERVE(FlameGun, HostTypeNrvRadiateOnly));
            break;
        }
    }
}

void FlameGun::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(GET_NERVE(FlameGun, HostTypeNrvPreRadiate));
    }
}

void FlameGun::exePreRadiate() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShootingBefore", nullptr);
        MR::startBrk(this, "ShootingBefore");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(FlameGun, HostTypeNrvRadiate));
        MR::setBrkFrameAndStop(this, 0.0f);
    }
}

void FlameGun::exeRadiate() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shooting", nullptr);
        MR::validateHitSensor(this, "attack");
    }

    MR::startLevelSound(this, "SE_EM_LV_FLAMEGUN_FIRE");

    if (MR::isGreaterStep(this, 120)) {
        setNerve(GET_NERVE(FlameGun, HostTypeNrvWait));
        MR::forceDeleteEffectAll(this);
        MR::invalidateHitSensor(this, "attack");
    }
}

void FlameGun::exeRotate() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shooting", nullptr);
        MR::validateHitSensor(this, "attack");
    }

    mRotation.y += 0.9f;
    MR::startLevelSound(this, "SE_EM_LV_FLAMEGUN_FIRE");

    if (!MR::isValidHitSensor(this, "attack")) {
        MR::validateHitSensor(this, "attack");
    }
}

void FlameGun::exeRadiateOnly() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shooting", nullptr);
        MR::validateHitSensor(this, "attack");
    }

    MR::startLevelSound(this, "SE_EM_LV_FLAMEGUN_FIRE");
}

// FlameGun::updateHitSensor

void FlameGun::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

FlameGun::~FlameGun() {
}
