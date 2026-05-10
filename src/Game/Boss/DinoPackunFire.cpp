#include "Game/Boss/DinoPackunFire.hpp"

namespace NrvDinoPackunFire {
    NEW_NERVE(DinoPackunFireNrvShot, DinoPackunFire, Shot);
    NEW_NERVE(DinoPackunFireNrvGround, DinoPackunFire, Ground);
};  // namespace NrvDinoPackunFire

DinoPackunFireHolder::DinoPackunFireHolder(s32 num) : DeriveActorGroup< DinoPackunFire >("ディノパックン炎管理", num) {
    for (s32 i = 0; i < num; i++) {
        DinoPackunFire* fire = new DinoPackunFire("ディノパックン炎");
        fire->initWithoutIter();
        registerActor(fire);
    }
}

DinoPackunFire::DinoPackunFire(const char* pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _8C.w = 1.0f;
}

void DinoPackunFire::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("DinoPackunFireBall", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    TVec3f attackOffs;
    attackOffs.set(0.0f, 0.0f, 0.0f);
    MR::addHitSensorEnemyAttack(this, "attack", 8, 150.0f, attackOffs);
    TVec3f starOffs;
    starOffs.set(0.0f);
    MR::initStarPointerTarget(this, 150.0f, starOffs);
    initBinder(150.0f, 0.0f, 0);
    initNerve(&NrvDinoPackunFire::DinoPackunFireNrvShot::sInstance);
    MR::startBtk(this, "DinoPackunFireBall");
    MR::initShadowVolumeSphere(this, 150.0f);
    MR::onCalcShadow(this, nullptr);
    MR::onCalcGravity(this);
    MR::invalidateClipping(this);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    makeActorDead();
}

void DinoPackunFire::kill() {
    MR::emitEffect(this, "Break");
    LiveActor::kill();
}

void DinoPackunFire::control() {
    return;
}

void DinoPackunFire::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

void DinoPackunFire::attackSensor(HitSensor* a1, HitSensor* a2) {
    MR::isSensorPlayer(a2);
    MR::sendMsgEnemyAttackFire(a2, a1);
}

void DinoPackunFire::appearShot(const TVec3f& rPos, const TVec3f& rVelocity) {
    mPosition.x = rPos.x;
    mPosition.y = rPos.y;
    mPosition.z = rPos.z;
    setNerve(&NrvDinoPackunFire::DinoPackunFireNrvShot::sInstance);
    appear();
    MR::setVelocity(this, rVelocity);
}

void DinoPackunFire::exeShot() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_D_PAKKUN_FIREBALL_L_SHOOT", -1, -1);
        MR::startAction(this, "Move");
    }

    TVec3f v5;
    JMathInlineVEC::PSVECNegate(&mGravity, &v5);
    MR::rotateQuatRollBall(&_8C, mVelocity, v5, (150.0f * mScale.x));
    MR::addVelocityToGravity(this, 2.0f);
    MR::attenuateVelocity(this, 0.98f);
    MR::reboundVelocityFromCollision(this, 0.80f, 15.0f, 0.40f);
    if (MR::isGreaterStep(this, 120) && MR::isBindedGround(this)) {
        setNerve(&NrvDinoPackunFire::DinoPackunFireNrvGround::sInstance);
        MR::zeroVelocity(this);
    } else {
        MR::startLevelSound(this, "SE_BM_LV_D_PAKKUN_FIREBALL_L", -1, -1, -1);
        MR::sendMsgEnemyAttackToBindedSensor(this, getSensor(nullptr));
        if (MR::isGreaterStep(this, 450)) {
            MR::startSound(this, "SE_BM_D_PAKKUN_FIREBALL_L_END", -1, -1);
            kill();
        }
    }
}

void DinoPackunFire::exeGround() {
    MR::zeroVelocity(this);
    MR::startLevelSound(this, "SE_BM_LV_D_PAKKUN_FIREBALL_L", -1, -1, -1);
    MR::sendMsgEnemyAttackToBindedSensor(this, getSensor(nullptr));
    if (MR::isGreaterStep(this, 20)) {
        MR::startSound(this, "SE_BM_D_PAKKUN_FIREBALL_L_END", -1, -1);
        kill();
    }
}

DinoPackunFire::~DinoPackunFire() {
}
