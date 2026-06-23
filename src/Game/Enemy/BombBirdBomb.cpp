#include "Game/Enemy/BombBirdBomb.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sGroundFric = 1.0f;
    // static const f32 sAirFric = _;
    static const f32 sMomentFric = 0.96f;
    static const f32 sGravityAccel = 1.2f;
    static const f32 sReboundRate = 0.4f;
    static const f32 sReboundSpeedMin = 5.0f;
    static const s32 sWaitTime = 150;
    static const s32 sExplosionMaxTime = 10;
    static const s32 sExplosionTime = 20;
    static const f32 sExplosionRadius = 280.0f;
    // static const f32 sBombBoundNoteOffVel = _;
    // static const f32 sBombBoundVolMinVel = _;
    // static const f32 sBombBoundVolMaxVel = _;
    // static const f32 hShiftingSandVel = _;
};  // namespace

namespace NrvBombBirdBomb {
    NEW_NERVE(HostTypeNrvHold, BombBirdBomb, Hold);
    NEW_NERVE(HostTypeNrvWait, BombBirdBomb, Wait);
    NEW_NERVE(HostTypeNrvExplosion, BombBirdBomb, Explosion);
};  // namespace NrvBombBirdBomb

BombBirdBomb::BombBirdBomb(const char* pName) : LiveActor(pName), _8C(0.0f, 0.0f, 0.0f, 1.0f), _9C(0.0f, 0.0f, 0.0f) {
}

void BombBirdBomb::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("BombBirdBomb", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::onCalcGravity(this);
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "bomb", 8, ::sExplosionRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateHitSensor(this, "bomb");
    initBinder(getSensor("body")->mRadius, 0.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(3, false);
    f32 radius = getSensor("body")->mRadius;
    MR::initShadowVolumeSphere(this, radius);
    initNerve(&NrvBombBirdBomb::HostTypeNrvHold::sInstance);
    MR::invalidateClipping(this);
    makeActorDead();
}

void BombBirdBomb::kill() {
    LiveActor::kill();
    MR::deleteEffect(this, "LocusSmoke");
}

void BombBirdBomb::control() {
    _9C.mult(::sMomentFric);
    MR::rotateQuatMoment(&_8C, _9C);
}

void BombBirdBomb::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
    MR::setBaseScale(this, mScale);
}

void BombBirdBomb::hold(const TVec3f& rPos) {
    MR::resetPosition(this, rPos);
    appear();
    MR::showModel(this);
    setNerve(&NrvBombBirdBomb::HostTypeNrvHold::sInstance);
}

void BombBirdBomb::start(const TVec3f& rTrans, const TVec3f& rVelocity) {
    mPosition.set(rTrans);
    MR::showModel(this);
    MR::validateHitSensor(this, "body");
    MR::invalidateHitSensor(this, "bomb");
    setNerve(&NrvBombBirdBomb::HostTypeNrvWait::sInstance);
    mVelocity.set(rVelocity);
    MR::emitEffect(this, "LocusSmoke");
}

void BombBirdBomb::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBombBirdBomb::HostTypeNrvExplosion::sInstance) && MR::isSensorEnemyAttack(pSender)) {
        MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
        return;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (!isNerve(&NrvBombBirdBomb::HostTypeNrvWait::sInstance)) {
        return;
    }

    if (!MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
        return;
    }

    MR::emitEffect(this, "Explosion");
    MR::startSound(this, "SE_EM_BOMB_EXPLOSION");
    kill();
}

bool BombBirdBomb::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool BombBirdBomb::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

void BombBirdBomb::exeHold() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Normal");
    }

    mVelocity.zero();
}

void BombBirdBomb::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Signal");
    }

    f32 dotP = mGravity.dot(mVelocity);
    TVec3f velocity;
    velocity.zeroInline();

    if (MR::calcVelocityAreaOrRailMoveOnGround(&velocity, this)) {
        TVec3f velocity2 = velocity;
        velocity2.mult(::sGroundFric);
        mVelocity.set(velocity);
        MR::addVelocityToGravity(this, ::sGravityAccel);

        if (dotP >= 5.5f) {
            MR::startSound(this, "SE_EM_BOMBBIRDBOMB_LAND_SAND");
        }
    } else {
        MR::addVelocityToGravity(this, ::sGravityAccel);

        if (MR::reboundVelocityFromCollision(this, ::sReboundRate, ::sReboundSpeedMin, 1.0f) && dotP >= 5.5f) {
            MR::startSound(this, "SE_EM_BOMBBIRDBOMB_LAND");
        }
    }

    if (MR::isGreaterStep(this, ::sWaitTime)) {
        setNerve(&NrvBombBirdBomb::HostTypeNrvExplosion::sInstance);
    }
}

void BombBirdBomb::exeExplosion() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::validateHitSensor(this, "bomb");
        MR::invalidateHitSensor(this, "body");
        MR::emitEffect(this, "Explosion");
        MR::startSound(this, "SE_EM_BOMB_EXPLOSION");
        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
        mVelocity.zero();
    }

    MR::setSensorRadius(this, "bomb", MR::calcNerveEaseOutValue(this, ::sExplosionMaxTime, 0.0f, ::sExplosionRadius));

    if (MR::isGreaterStep(this, ::sExplosionTime)) {
        MR::invalidateHitSensor(this, "bomb");
        kill();
    }
}
