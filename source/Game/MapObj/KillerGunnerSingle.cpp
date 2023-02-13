#include "Game/MapObj/KillerGunnerSingle.h"

KillerGunnerSingle::KillerGunnerSingle(const char *pName) : LiveActor(pName) {

}

void KillerGunnerSingle::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("KillerGunnerSingle", nullptr, false);
    MR::connectToSceneEnvironment(this);
    initHitSensor(2);
    TVec3f sensor_offs;
    sensor_offs.x = 0.0f;
    sensor_offs.y = 0.0f;
    sensor_offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 0x10, 0.0f, sensor_offs);

    TVec3f shell_sensor_offs(sShellPos);
    MR::addHitSensorEnemy(this, "shell", 0x10, 250.0f, shell_sensor_offs);
    MR::initCollisionParts(this, "KillerGunnerSingle", getSensor("body"), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    initNerve(&NrvKillerGunnerSingle::HostTypeWait::sInstance);
    makeActorAppeared();
}

void KillerGunnerSingle::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (isNerve(&NrvKillerGunnerSingle::HostTypeShoot::sInstance)) {
        if (MR::isSensorPlayer(a2)) {
            if (MR::isNear(a1, a2, a1->mRadius)) {
                if (MR::sendMsgEnemyAttackMaximum(a2, a1)) {
                    setNerve(&NrvKillerGunnerSingle::HostTypeAttack::sInstance);
                }
            }
        }
    }
}

void KillerGunnerSingle::exeWait() {
    if (MR::isFirstStep(this)) {
        getSensor("shell")->invalidate();
    }

    if (MR::isStep(this, 120)) {
        setNerve(&NrvKillerGunnerSingle::HostTypeCharge::sInstance);
    }
}

void KillerGunnerSingle::exeCharge() {
    if (MR::isFirstStep(this)) {
        MR::startBtk(this, "KillerGunnerSingleCharge");
    }

    MR::startLevelSound(this, "SE_OJ_LV_K_GUNNER_CHARGE", -1, -1, -1);

    if (MR::isBtkStopped(this)) {
        MR::startBck(this, "KillerGunnerSingleShoot", nullptr);
        MR::emitEffect(this, "KillerGunnerSingleSmoke");
        MR::startSound(this, "SE_OJ_K_GUNNER_FIRE", -1, -1);
        setNerve(&NrvKillerGunnerSingle::HostTypeShoot::sInstance);
    }
}

void KillerGunnerSingle::exeShoot() {
    if (MR::isFirstStep(this)) {
        MR::startRumbleWithShakeCameraWeak(this, "中", "弱", 500.0f, 1000.0f);
    }

    if (MR::isStep(this, 0)) {
        getSensor("shell")->validate();
    }

    if (MR::isStep(this, 40)) {
        getSensor("shell")->invalidate();
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvKillerGunnerSingle::HostTypeWait::sInstance);
    }
}

inline void KillerGunnerSingle::exeAttack() {
    if (MR::isBckStopped(this)) {
        setNerve(&NrvKillerGunnerSingle::HostTypeWait::sInstance);
    }
}

KillerGunnerSingle::~KillerGunnerSingle() {
    
}

namespace NrvKillerGunnerSingle {
    INIT_NERVE(HostTypeAttack);
    INIT_NERVE(HostTypeShoot);
    INIT_NERVE(HostTypeCharge);
    INIT_NERVE(HostTypeWait);
};