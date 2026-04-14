#include "Game/Enemy/KameckFireBall.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"

namespace {
    static Color8 sPointLightColor(0xFF, 0x64, 0, 0xFF);
};  // namespace

namespace NrvBossKameckBeamFire {
    NEW_NERVE(KameckFireBallNrvWait, KameckFireBall, Wait);
    NEW_NERVE(KameckFireBallNrvSweep, KameckFireBall, Sweep);
};  // namespace NrvBossKameckBeamFire

KameckFireBall::KameckFireBall(const char* pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _8C.w = 1.0f;
    mEventListener = nullptr;
}

void KameckFireBall::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("BossKameckFireBall", nullptr, false);
    MR::connectToSceneEnemy(this);
    initNerve(&NrvBossKameckBeamFire::KameckFireBallNrvWait::sInstance);
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 0.0f;
    sensorOffs.z = 0.0f;
    MR::addHitSensorEnemyAttack(this, "attack", 8, 58.0f, sensorOffs);
    TVec3f pointerOffs;
    pointerOffs.x = 0.0f;
    pointerOffs.y = 0.0f;
    pointerOffs.z = 0.0f;
    MR::initStarPointerTarget(this, 58.0f, pointerOffs);
    initBinder(58.0f, 0.0f, 0);
    MR::setKameckBeamCollisionFilter(this);
    MR::initShadowVolumeSphere(this, 58.0f);
    MR::onCalcShadow(this, nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::invalidateClipping(this);
    makeActorDead();
}

void KameckFireBall::kill() {
    MR::startSound(this, "SE_BM_KAMECK_DISAPPEAR_FIRE", -1, -1);
    MR::emitEffect(this, "BeamFireVanish");
    LiveActor::kill();
}

void KameckFireBall::makeActorAppeared() {
    MR::showModel(this);
    LiveActor::makeActorAppeared();
    setNerve(&NrvBossKameckBeamFire::KameckFireBallNrvWait::sInstance);
}

void KameckFireBall::makeActorDead() {
    LiveActor::makeActorDead();
    mEventListener = nullptr;
}

void KameckFireBall::control() {
    MR::requestPointLight(this, TVec3f(mPosition), sPointLightColor, 1.0f, -1);
}

void KameckFireBall::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
    MR::setBaseScale(this, mScale);
}

void KameckFireBall::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBossKameckBeamFire::KameckFireBallNrvWait::sInstance)) {
        if (MR::isSensorEnemyAttack(pSender) && MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
            if (mEventListener != nullptr) {
                mEventListener->hitBeam(2);
            }

            kill();
        }
    }
}

bool KameckFireBall::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg) && isNerve(&NrvBossKameckBeamFire::KameckFireBallNrvWait::sInstance)) {
        kill();
        return true;
    }

    return false;
}

bool KameckFireBall::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStartPowerStarGet(msg)) {
        makeActorDead();
        return true;
    }

    return false;
}

/*
void KameckFireBall::appearDirection(const TVec3f& rDirection) {
    appear();
    MR::onCalcGravity(this);
    MR::calcGravity(this);
}*/

void KameckFireBall::setEventListener(KameckBeamEventListener* pListener) {
    mEventListener = pListener;
}

void KameckFireBall::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "BossKameckFireBall");
    }

    MR::startLevelSound(this, "SE_BM_LV_KAMECK_FIRE_BALL", -1, -1, -1);
    MR::rotateQuatRollBall(&_8C, mVelocity, -mGravity, 58.0f);
    MR::addVelocityToGravity(this, 0.5f);
    MR::attenuateVelocity(this, 0.995f);

    if (MR::reboundVelocityFromCollision(this, 0.40f, 0.0f, 1.0f)) {
        if (mVelocity.dot(-mGravity) >= 1.0f) {
            MR::startSound(this, "SE_BM_KAMECK_FIRE_BOUND", -1, -1);
        }
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", 1, 0)) {
        kill();
        return;
    }

    if (MR::isGreaterStep(this, 180) || MR::isInWater(mPosition)) {
        kill();
    }
}

void KameckFireBall::exeSweep() {
    if (MR::isFirstStep(this)) {
        MR::hideModelAndOnCalcAnim(this);
    }

    if (MR::isGreaterStep(this, 40)) {
        kill();
    }
}

KameckFireBall::~KameckFireBall() {
}
