#include "Game/MapObj/FirePressureBullet.h"

FirePressureBullet::FirePressureBullet(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    mFirePressure = NULL;
    _9C = 0.0f;
    _A0 = false;
    _A1 = false;
}

void FirePressureBullet::init(const JMapInfoIter  &rIter) {
    initModelManagerWithAnm("FireBullet", NULL, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensorEnemyAttack(this, "body", 8, 100.0f, offs);
    initBinder(100.0f, 0.0f, 0);
    MR::onCalcGravity(this);
    initEffectKeeper(0, NULL, false);
    initSound(4, false);
    MR::initShadowVolumeSphere(this, 75.0f);
    initNerve(&NrvFirePressureBullet::FirePressureBulletNrvFly::sInstance);
    makeActorDead();
}

void FirePressureBullet::kill() {
    MR::startSound(this, "SE_OJ_F_PRESSURE_FIRE_BURST", -1, -1);
    LiveActor::kill();
}

void FirePressureBullet::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this,mGravity, _8C);
    MR::setBaseTRMtx(this, mtx);
}

void FirePressureBullet::shotFireBullet(LiveActor *pActor, const TPos3f &rPos, const f32 &a3, bool a4, bool a5) {
    f32 v7 = rPos.mMtx[2][2];
    f32 v9 = rPos.mMtx[1][2];
    f32 v11 = rPos.mMtx[0][2];
    mFirePressure = pActor;
    _9C = a3;
    _A0 = a4;
    _A1 = a5;
    _8C.set(v11, v9, v7);
    mVelocity.scale(_9C, _8C);
    f32 z = rPos.mMtx[2][3];
    f32 y = rPos.mMtx[1][3];
    f32 x = rPos.mMtx[0][3];
    mPosition.set(x, y, z);
    mRotation.z = 0.0f;
    mRotation.y = 0.0f;
    mRotation.x = 0.0f;
    MR::validateHitSensors(this);
    MR::invalidateClipping(this);
    setNerve(&NrvFirePressureBullet::FirePressureBulletNrvFly::sInstance);
    makeActorAppeared();
}

void FirePressureBullet::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shot", NULL);
        MR::startBtk(this, "FireBullet");
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "Move", NULL);
    }

    MR::startLevelSound(this, "SE_OJ_LV_F_PRESSURE_BULLET", -1, -1, -1);

    if (_A0 && !(getNerveStep() % 5)) {
        MR::turnDirectionToTargetDegree(this, &_8C, *MR::getPlayerPos(), 3.0f);
        mVelocity.scale(_9C, _8C);
    }
    
    if (isCrash()) {
        kill();
    }
    else if (MR::isStep(this, 300)) {
        kill();
    }
}

void FirePressureBullet::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorEnemy(a2) || MR::isSensorMapObj(a2)) {
        kill();
    }
    else if (!MR::isSensorPlayer(a2) && MR::sendMsgEnemyAttackFire(a2, a1)) {
        kill();
    }
}

bool FirePressureBullet::isCrash() const {
    if (_A1) {
        return false;
    }

    return MR::isBinded(this);
}

FirePressureBullet::~FirePressureBullet() {

}

namespace NrvFirePressureBullet {
    INIT_NERVE(FirePressureBulletNrvFly);
};
