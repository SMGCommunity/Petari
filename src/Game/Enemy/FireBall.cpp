#include "Game/Enemy/FireBall.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const f32 cSensorRadius = 40.0f;
    const f32 cBinderRadius = 60.0f;
    const f32 cForceKillDistance = 5000.0f;
    const f32 cStarWandRadius3d = 150.0f;
    const f32 cThrowOffsetY = 120.0f;
    const f32 cReflectSpeed = 100.0f;
    const f32 cReflectReduceRate = 0.96f;
    const s32 cReflectFrame = 30;
    const s32 cReflectInvalidFrame = 60;
    const f32 cReflectCursorSpeed = 30.0f;
    const f32 cReflectRandomVelocityY = 1.0f;
};  // namespace

namespace NrvFireBall {
    NEW_NERVE(FireBallNrvThrow, FireBall, Throw);
    NEW_NERVE(FireBallNrvReflect, FireBall, Reflect);
};  // namespace NrvFireBall

FireBall::FireBall(const char* pName) : LiveActor(pName), mHost(), mUp(0.0f, 1.0f, 0.0f) {
}

void FireBall::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("FireBall", nullptr, false);

    MR::connectToSceneEnemyDecoration(this);

    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 8, ::cSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));

    initBinder(::cBinderRadius, 0.0f, 0);

    initEffectKeeper(4, "FireBall", false);
    
    MR::initStarPointerTarget(this, ::cStarWandRadius3d, TVec3f(0.0f, 0.0f, 0.0f));
    
    initSound(6, false);
    
    MR::initShadowVolumeCylinder(this, ::cBinderRadius);

    MR::invalidateClipping(this);

    initNerve(GET_NERVE(FireBall, FireBallNrvThrow));

    makeActorDead();
}

void FireBall::appear() {
    LiveActor::appear();
    setNerve(GET_NERVE(FireBall, FireBallNrvThrow));
}

void FireBall::kill() {
    MR::forceDeleteEffect(this, "FireBall");

    MR::emitEffect(this, "FireBallBreak");

    MR::startSound(this, "SE_OJ_FIRE_BALL_BREAK");

    LiveActor::kill();
}

void FireBall::appearAndThrow(const TVec3f& rAppearPos, f32 speed, f32 angle) {
    mPosition.set(rAppearPos);

    MR::calcGravity(this);

    mUp.negate(mGravity);

    setVelocityToPlayer(speed);

    TPos3f mtx;
    mtx.makeRotate(mUp, MR::toRadian(angle));
    mtx.mult33(mVelocity, mVelocity);

    appear();
}

void FireBall::control() {
    GXColor color = {255, 192, 0, 255};
    MR::requestPointLight(this, mPosition, color, 1.0f, -1);
}

void FireBall::calcAndSetBaseMtx() {
    if (!MR::isNearZero(mVelocity)) {
        TVec3f frontVec;
        TPos3f baseMtx;
        MR::normalize(mVelocity, &frontVec);
        MR::makeMtxFrontUpPos(&baseMtx, frontVec, mUp, mPosition);
        MR::setBaseTRMtx(this, baseMtx);
    }
}

void FireBall::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
        kill();
        return;
    }

    if (MR::isSensorEnemy(pReceiver) && pReceiver->mHost != mHost && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
        kill();
    }
}

bool FireBall::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgJetTurtleAttack(msg)) {
        kill();
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        kill();
        return true;
    }

    return false;
}

HitSensor* FireBall::isBindedAny() const {
    if (MR::isBindedGround(this)) {
        return MR::getGroundSensor(this);
    }

    if (MR::isBindedWall(this)) {
        return MR::getWallSensor(this);
    }

    if (MR::isBindedRoof(this)) {
        return MR::getRoofSensor(this);
    }

    return nullptr;
}

bool FireBall::tryToKill() {
    HitSensor* bindedSensor = isBindedAny();

    if (bindedSensor != nullptr) {
        MR::sendMsgEnemyAttack(bindedSensor, getSensor("body"));

        kill();

        return true;
    }

    if (!MR::isNearPlayer(this, ::cForceKillDistance)) {
        kill();
        return true;
    }

    return false;
}

void FireBall::setVelocityToPlayer(f32 magnitude) {
    TVec3f target;
    target.scale(::cThrowOffsetY, mUp);
    target.add(*MR::getPlayerPos());

    TVec3f dir;
    dir.sub(target, mPosition);
    MR::normalize(&dir);
    mVelocity.scale(magnitude, dir);
}

void FireBall::calcReflectVelocity() {
    MR::getStarPointerWorldVelocityDirection(&mVelocity, *MR::getStarPointerLastPointedPort(this));

    if (MR::isNearZero(mVelocity)) {
        mVelocity.set(mUp);
    } else {
        MR::vecKillElement(mVelocity, mUp, &mVelocity);

        TVec3f accel;
        accel.scale(MR::getRandom(0.0f, ::cReflectRandomVelocityY), mUp);
        mVelocity.add(accel);

        if (MR::isNearZero(mVelocity)) {
            mVelocity.set(mUp);
        } else {
            MR::normalize(&mVelocity);
        }
    }

    mVelocity.mult(::cReflectSpeed);
}

void FireBall::exeThrow() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Spin", nullptr);
    }

    if (MR::changeShowModelFlagSyncNearClipping(this, 200.0f)) {
        MR::emitEffect(this, "FireBall");
    } else {
        MR::deleteEffect(this, "FireBall");
    }

    if (MR::isGreaterStep(this, ::cReflectFrame) && MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        TVec2f screenVelocity(*MR::getStarPointerScreenVelocity(*MR::getStarPointerLastPointedPort(this)));
        if (::cReflectCursorSpeed < screenVelocity.length()) {
            calcReflectVelocity();

            setNerve(GET_NERVE(FireBall, FireBallNrvReflect));

            return;
        }
    }

    if (tryToKill()) {
        return;
    }
}

void FireBall::exeReflect() {
    if (MR::isFirstStep(this)) {
        MR::start2PAttackAssistSound();
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_REFLECT");
    }

    mVelocity.mult(::cReflectReduceRate);

    if (!tryToKill() && MR::isStep(this, ::cReflectInvalidFrame)) {
        kill();
    }
}
