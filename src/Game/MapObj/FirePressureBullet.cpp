#include "Game/MapObj/FirePressureBullet.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void FirePressureBullet_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)3.0f;
}

namespace NrvFirePressureBullet {
    NEW_NERVE(FirePressureBulletNrvFly, FirePressureBullet, Fly);
};  // namespace NrvFirePressureBullet

FirePressureBullet::FirePressureBullet(const char* pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    mFirePressure = nullptr;
    _9C = 0.0f;
    _A0 = false;
    _A1 = false;
}

void FirePressureBullet::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("FireBullet", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensorEnemyAttack(this, "body", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(100.0f, 0.0f, 0);
    MR::onCalcGravity(this);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::initShadowVolumeSphere(this, 75.0f);
    initNerve(GET_NERVE(FirePressureBullet, FirePressureBulletNrvFly));
    makeActorDead();
}

void FirePressureBullet::kill() {
    MR::startSound(this, "SE_OJ_F_PRESSURE_FIRE_BURST");
    LiveActor::kill();
}

void FirePressureBullet::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, _8C);
    MR::setBaseTRMtx(this, mtx);
}

void FirePressureBullet::shotFireBullet(LiveActor* pActor, const TPos3f& rPos, const f32& a3, bool a4, bool a5) {
    mFirePressure = pActor;
    _9C = a3;
    _A0 = a4;
    _A1 = a5;

    rPos.getZDir(_8C);
    mVelocity.scale(_9C, _8C);
    rPos.getTrans(mPosition);
    mRotation.zero();
    MR::validateHitSensors(this);
    MR::invalidateClipping(this);
    setNerve(GET_NERVE(FirePressureBullet, FirePressureBulletNrvFly));
    makeActorAppeared();
}

void FirePressureBullet::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shot");
        MR::startBtk(this, "FireBullet");
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "Move");
    }

    MR::startLevelSound(this, "SE_OJ_LV_F_PRESSURE_BULLET");

    if (_A0 && !(getNerveStep() % 5)) {
        MR::turnDirectionToTargetDegree(this, &_8C, *MR::getPlayerPos(), 3.0f);
        mVelocity.scale(_9C, _8C);
    }

    if (isCrash()) {
        kill();
    } else if (MR::isStep(this, 300)) {
        kill();
    }
}

void FirePressureBullet::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemy(pReceiver) || MR::isSensorMapObj(pReceiver)) {
        kill();
    } else if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
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
