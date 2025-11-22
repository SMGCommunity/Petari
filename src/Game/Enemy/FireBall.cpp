#include "Game/Enemy/FireBall.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace NrvFireBall {
    NEW_NERVE(FireBallNrvThrow, FireBall, Throw);
    NEW_NERVE(FireBallNrvReflect, FireBall, Reflect);
}; // namespace NrvFireBall

FireBall::FireBall(const char* pName)
    : LiveActor(pName),
      _8C(nullptr),
      _90(0.0f, 1.0f, 0.0f) {}

void FireBall::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("FireBall", nullptr, false);
    MR::connectToSceneEnemyDecoration(this);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 8, 40.0, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(60.0f, 0.0f, 0);
    initEffectKeeper(4, "FireBall", false);
    MR::initStarPointerTarget(this, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initSound(6, false);
    MR::initShadowVolumeCylinder(this, 60.0f);
    MR::invalidateClipping(this);
    initNerve(&NrvFireBall::FireBallNrvThrow::sInstance);
    makeActorDead();
}

void FireBall::appear() {
    LiveActor::appear();
    setNerve(&NrvFireBall::FireBallNrvThrow::sInstance);
}

void FireBall::kill() {
    MR::forceDeleteEffect(this, "FireBall");
    MR::emitEffect(this, "FireBallBreak");
    MR::startSound(this, "SE_OJ_FIRE_BALL_BREAK", -1, -1);
    LiveActor::kill();
}

//FireBall::appearAndThrow(const TVec3f &, f32, f32)

void FireBall::control() {
    Color8 color;
    color.set(255, 192, 0, 255);
    MR::requestPointLight(this, mPosition, color, 1.0f, -1);
}

void FireBall::calcAndSetBaseMtx() {
    if (!MR::isNearZero(mVelocity, 0.001f)) {
        TVec3f stack_8;
        TPos3f stack_14;
        MR::normalize(mVelocity, &stack_8);
        MR::makeMtxFrontUpPos(&stack_14, stack_8, _90, mPosition);
        MR::setBaseTRMtx(this, stack_14);
    }
}

void FireBall::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
        kill();
        return;
    }
    if (MR::isSensorEnemy(pReceiver) && pReceiver->mHost != this->_8C && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
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

    if (bindedSensor) {
        MR::sendMsgEnemyAttack(bindedSensor, getSensor("body"));
        kill();
        return true;
    }
    if (!MR::isNearPlayer(this, 5000.0f)) {
        kill();
        return true;
    }

    return false;
}

void FireBall::setVelocityToPlayer(f32 param1) {
    TVec3f stack_14;
    stack_14.scale(120.0f, _90);
    stack_14.add(*MR::getPlayerPos());
    TVec3f a1;
    JMathInlineVEC::PSVECSubtract2(&stack_14, &mPosition, &a1);
    MR::normalize(&a1);
    mVelocity.scale(param1, a1);
}

void FireBall::calcReflectVelocity() {
    MR::getStarPointerWorldVelocityDirection(&mVelocity, *MR::getStarPointerLastPointedPort(this));
    if (MR::isNearZero(mVelocity, 0.001f)) {
        mVelocity.set(_90);
    } else {
        MR::vecKillElement(mVelocity, _90, &mVelocity);
        TVec3f v7;
        v7.scale(MR::getRandom(0.0f, 1.0f), _90);
        mVelocity.add(v7);
        if (MR::isNearZero(mVelocity, 0.001f)) {
            mVelocity.set(_90);
        } else {
            MR::normalize(&mVelocity);
        }
    }
    mVelocity.mult(100.0f);
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

    if (MR::isGreaterStep(this, 30) && MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        s32*   starPointerLastPointedPort = MR::getStarPointerLastPointedPort(this);
        TVec2f pointerScreenVel = *MR::getStarPointerScreenVelocity(*starPointerLastPointedPort);
        if (30.0f < pointerScreenVel.length()) {
            calcReflectVelocity();
            setNerve(&NrvFireBall::FireBallNrvReflect::sInstance);
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
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_REFLECT", -1, -1);
    }
    mVelocity.mult(0.96f);

    if (!tryToKill()) {
        if (MR::isStep(this, 60)) {
            kill();
        }
    }
}
