#include "Game/Enemy/KameckFireBall.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    static const f32 sFireRadius = 58.0f;
    static const s32 sLife = 180;
    static const f32 sInitVerticalSpeed = 15.0f;
    static const f32 sInitHorizonSpeed = 15.0f;
    static const f32 sGravityAccel = 0.5f;
    static const f32 sFreqRate = 0.995f;
    static const f32 sReboundRate = 0.4f;
    static const s32 sSweepTime = 40;
    static Color8 sPointLightColor(0xFF, 0x64, 0, 0xFF);
};  // namespace

namespace NrvBossKameckBeamFire {
    NEW_NERVE(KameckFireBallNrvWait, KameckFireBall, Wait);
    NEW_NERVE(KameckFireBallNrvSweep, KameckFireBall, Sweep);
};  // namespace NrvBossKameckBeamFire

KameckFireBall::KameckFireBall(const char* pName) : LiveActor(pName), mRotateQuat(0.0f, 0.0f, 0.0f, 1.0f), mEventListener() {
}

void KameckFireBall::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("BossKameckFireBall", nullptr, false);
    MR::connectToSceneEnemy(this);
    initNerve(GET_NERVE(BossKameckBeamFire, KameckFireBallNrvWait));
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 0.0f;
    sensorOffs.z = 0.0f;
    MR::addHitSensorEnemyAttack(this, "attack", 8, ::sFireRadius, sensorOffs);
    TVec3f pointerOffs;
    pointerOffs.x = 0.0f;
    pointerOffs.y = 0.0f;
    pointerOffs.z = 0.0f;
    MR::initStarPointerTarget(this, ::sFireRadius, pointerOffs);
    initBinder(::sFireRadius, 0.0f, 0);
    MR::setKameckBeamCollisionFilter(this);
    MR::initShadowVolumeSphere(this, ::sFireRadius);
    MR::onCalcShadow(this, nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::invalidateClipping(this);
    makeActorDead();
}

void KameckFireBall::kill() {
    MR::startSound(this, "SE_BM_KAMECK_DISAPPEAR_FIRE");
    MR::emitEffect(this, "BeamFireVanish");
    LiveActor::kill();
}

void KameckFireBall::makeActorAppeared() {
    MR::showModel(this);
    LiveActor::makeActorAppeared();
    setNerve(GET_NERVE(BossKameckBeamFire, KameckFireBallNrvWait));
}

void KameckFireBall::makeActorDead() {
    LiveActor::makeActorDead();
    mEventListener = nullptr;
}

void KameckFireBall::control() {
    MR::requestPointLight(this, TVec3f(mPosition), ::sPointLightColor, 1.0f, -1);
}

void KameckFireBall::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mRotateQuat);
    MR::setBaseScale(this, mScale);
}

void KameckFireBall::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(BossKameckBeamFire, KameckFireBallNrvWait))) {
        if (MR::isSensorEnemyAttack(pSender) && MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
            if (mEventListener != nullptr) {
                mEventListener->hitBeam(KameckBeam::BeamType_FireBall1);
            }

            kill();
        }
    }
}

bool KameckFireBall::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg) && isNerve(GET_NERVE(BossKameckBeamFire, KameckFireBallNrvWait))) {
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

void KameckFireBall::appearDirection(const TVec3f& rDirection) {
    appear();
    MR::onCalcGravity(this);
    MR::calcGravity(this);
    mVelocity.set(rDirection * ::sInitHorizonSpeed + mGravity * ::sInitVerticalSpeed);
}

void KameckFireBall::setEventListener(KameckBeamEventListener* pListener) {
    mEventListener = pListener;
}

void KameckFireBall::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "BossKameckFireBall");
    }

    MR::startLevelSound(this, "SE_BM_LV_KAMECK_FIRE_BALL");
    MR::rotateQuatRollBall(&mRotateQuat, mVelocity, -mGravity, ::sFireRadius);
    MR::addVelocityToGravity(this, ::sGravityAccel);
    MR::attenuateVelocity(this, ::sFreqRate);

    if (MR::reboundVelocityFromCollision(this, ::sReboundRate, 0.0f, 1.0f)) {
        if (mVelocity.dot(-mGravity) >= 1.0f) {
            MR::startSound(this, "SE_BM_KAMECK_FIRE_BOUND");
        }
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", 1, 0)) {
        kill();
    } else if (MR::isGreaterStep(this, ::sLife) || MR::isInWater(mPosition)) {
        kill();
    }
}

void KameckFireBall::exeSweep() {
    if (MR::isFirstStep(this)) {
        MR::hideModelAndOnCalcAnim(this);
    }

    if (MR::isGreaterStep(this, ::sSweepTime)) {
        kill();
    }
}
