#include "Game/Boss/PoltaRock.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sModelSize = 120.0f;
    static const s32 sForceBreakTime = 300;
    static const s32 sFloatTime = 90;
    static const f32 sFloatHeight = 800.0f;
    static const f32 sFloatToTargetAccel = 1.2f;
    static const f32 sFloatAccelRange = 300.0f;
    static const f32 sFloatFric = 0.92f;
    static const s32 sSignTime = 60;
    static const f32 sMoveAccel = 3.0f;
    static const s32 sAccelTime = 30;
    static const f32 sGravityAccel = -0.02f;
    static const f32 sNormalFric = 0.999f;
};  // namespace

namespace NrvPoltaRock {
    NEW_NERVE(PoltaRockNrvFloat, PoltaRock, Float);
    NEW_NERVE(PoltaRockNrvSign, PoltaRock, Sign);
    NEW_NERVE(PoltaRockNrvMove, PoltaRock, Move);
};  // namespace NrvPoltaRock

PoltaRock::PoltaRock(const char* pName)
    : LiveActor(pName), mHost(), mQuat(0.0f, 0.0f, 0.0f, 1.0f), mTargetPosition(0.0f, 0.0f, 1.0f), mOffset(0.0f, 0.0f, 0.0f), mType() {
}

void PoltaRock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("PoltaRock", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::onCalcGravity(this);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "Body", 8, ::sModelSize, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(::sModelSize, 0.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::initShadowFromCSV(this, "Shadow");
    initNerve(GET_NERVE(PoltaRock, PoltaRockNrvFloat));
    MR::invalidateClipping(this);
    makeActorDead();
}

void PoltaRock::kill() {
    MR::startSound(this, "SE_OJ_POLTA_ROCK_BREAK");
    LiveActor::kill();
    switch (mType) {
    case 2:
        MR::emitEffect(this, "BreakYellow");
        break;
    case 0:
        MR::emitEffect(this, "BreakWhite");
        break;
    }
    mHost = nullptr;
}

void PoltaRock::control() {
}

void PoltaRock::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mQuat);
    MR::setBaseScale(this, mScale);
}

void PoltaRock::start(Polta* pHost, const TVec3f& rPosition, const TVec3f& rOffset) {
    mHost = pHost;
    mPosition.set(rPosition);
    mOffset.set(rOffset);
    setNerve(GET_NERVE(PoltaRock, PoltaRockNrvFloat));
    appear();
    MR::offBind(this);
}

void PoltaRock::setColorWhite() {
    mType = 0;
    MR::startBva(this, "Color");
    MR::setBvaFrameAndStop(this, 0.0f);
}

void PoltaRock::setColorBlack() {
    mType = 1;
    MR::startBva(this, "Color");
    MR::setBvaFrameAndStop(this, 1.0f);
}

void PoltaRock::setColorYellow() {
    mType = 2;
    MR::startBva(this, "Color");
    MR::setBvaFrameAndStop(this, 2.0f);
}

void PoltaRock::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (isEnableAttack()) {
            if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                generateKill();
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool PoltaRock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }
    if (MR::isMsgStarPieceAttack(msg)) {
        MR::zeroVelocity(this);
        generateKill();
        return true;
    }
    if (MR::isMsgPlayerSpinAttack(msg) && isEnableAttack()) {
        MR::zeroVelocity(this);
        generateKill();
        return true;
    }
    return false;
}

bool PoltaRock::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg) && isEnableExplosion()) {
        MR::zeroVelocity(this);
        generateKill();
        return true;
    }
    return false;
}

void PoltaRock::exeFloat() {
    TVec3f target;

    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_POLTA_ROCK_FLOAT_ST");
        MR::startAction(this, "Froat");
        MR::emitEffect(this, "Appear");
    }

    MR::startLevelSound(this, "SE_OJ_LV_POLTA_ROCK_FLOAT");
    PSMTXMultVec(getHost()->getBaseMtx(), &mOffset, target);
    MR::addVelocityMoveToTarget(this, target, 0.0f, ::sFloatToTargetAccel, 0.0f, 200.0f);
    MR::addVelocityKeepHeightUseShadow(this, ::sFloatHeight, 1.5f, ::sFloatAccelRange, nullptr);
    MR::attenuateVelocity(this, ::sFloatFric);
    if (MR::isGreaterStep(this, ::sFloatTime)) {
        setNerve(GET_NERVE(PoltaRock, PoltaRockNrvSign));
        MR::onBind(this);
    }
}

void PoltaRock::exeSign() {
    TVec3f target;

    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Sign");
    }

    PSMTXMultVec(getHost()->getBaseMtx(), &mOffset, target);
    MR::addVelocityMoveToTarget(this, target, 0.0f, ::sFloatToTargetAccel, 0.0f, 200.0f);
    MR::addVelocityKeepHeightUseShadow(this, ::sFloatHeight, 1.5f, ::sFloatAccelRange, nullptr);
    MR::attenuateVelocity(this, ::sFloatFric);
    if (MR::isGreaterStep(this, ::sSignTime)) {
        setNerve(GET_NERVE(PoltaRock, PoltaRockNrvMove));
        MR::onBind(this);
    }
}

void PoltaRock::exeMove() {
    if (MR::isFirstStep(this)) {
        mTargetPosition.set(*MR::getPlayerCenterPos() - mPosition);
        MR::normalizeOrZero(&mTargetPosition);
        MR::startSound(this, "SE_OJ_POLTA_ROCK_FLY");
        MR::zeroVelocity(this);
    }

    if (MR::isLessStep(this, ::sAccelTime)) {
        mVelocity.add(mTargetPosition * (::sMoveAccel * MR::calcNerveRate(this, ::sAccelTime)));
    }
    MR::addVelocityToGravity(this, ::sGravityAccel);
    MR::attenuateVelocity(this, ::sNormalFric);
    MR::rotateQuatRollBall(&mQuat, mVelocity, -mGravity, ::sModelSize);
    if (MR::isBinded(this)) {
        MR::sendMsgEnemyAttackToBindedSensor(this, getSensor("Body"));
        MR::tryRumbleDefaultHit(this, WPAD_CHAN0);
        generateKill();
        return;
    }

    if (MR::isGreaterStep(this, ::sForceBreakTime)) {
        kill();
    }
}

void PoltaRock::generateKill() {
    switch (mType) {
    case 1:
        PoltaFunction::appearBombTeresaNormal(getHost(), mPosition, TVec3f(0.0f, 0.0f, 0.0f));
        break;
    case 2:
        MR::appearCoinPop(getHost(), mPosition, 1);
        break;
    }
    kill();
}

bool PoltaRock::isEnableAttack() const {
    return isNerve(GET_NERVE(PoltaRock, PoltaRockNrvMove));
}

bool PoltaRock::isEnableExplosion() const {
    if (isNerve(GET_NERVE(PoltaRock, PoltaRockNrvFloat)) && MR::isLessStep(this, 200)) {
        return false;
    }
    return true;
}

PoltaRock::~PoltaRock() {
}
