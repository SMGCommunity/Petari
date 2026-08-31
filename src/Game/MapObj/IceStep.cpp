#include "Game/MapObj/IceStep.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvIceStep {
    NEW_NERVE(IceStepNrvWait, IceStep, Wait);
    NEW_NERVE(IceStepNrvHit, IceStep, Hit);
};  // namespace NrvIceStep

void IceStep_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)90.0f;
    (void)0.2f;
}

IceStep::IceStep(const char* pName) : LiveActor(pName) {
}

IceStep::~IceStep() {
}

void IceStep::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("IceStep", nullptr, false);

    MR::connectToSceneMapObjStrongLight(this);

    initSound(4, false);
    initEffectKeeper(1, nullptr, false);

    f32 radius = 90.0f * mScale.x;
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, radius, TVec3f(0.0f, radius, 0.0f));

    PSMTXIdentity(_8C);

    MR::initCollisionParts(this, "IceStep", getSensor("body"), nullptr);

    MR::validateClipping(this);

    initNerve(GET_NERVE(IceStep, IceStepNrvWait));

    kill();

    mTimer = 0;
}

void IceStep::exeWait() {
    if (mTimer != 0) {
        mTimer--;
    }

    switch (mTimer) {
    case 300:
        MR::startBpk(this, "TimeOutLong");

        break;
    case 120:
        MR::startBpk(this, "TimeOutShort");

        break;
    case 1:
        destroy();

        break;
    case 0:
        if (MR::isBckStopped(this)) {
            kill();
        }

        break;
    }
}

void IceStep::exeHit() {
    switch (getNerveStep()) {
    case 5:
        MR::hideModel(this);

        break;
    case 15:
        kill();

        break;
    }
}

void IceStep::setOn(u32 a1, const TVec3f& rPosition, const TVec3f& rRotation) {
    mPosition = rPosition;
    mRotation = rRotation;

    MR::offUpdateCollisionParts(this);
    MR::onUpdateCollisionPartsOnetimeImmediately(this);

    s32 scale = a1 / 2;
    if (a1 % 2) {
        scale -= 10;
    }

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    TVec3f offset = upVec * static_cast< f32 >(scale) * 0.2f;
    mPosition = rPosition + offset;

    MR::startBck(this, "Start", nullptr);
    MR::startBpk(this, "Start");
    MR::startSound(this, "SE_OJ_ICE_FLOOR_FREEZE");

    mTimer = 1200;

    MR::invalidateClipping(this);

    MR::resetPosition(this);

    setNerve(GET_NERVE(IceStep, IceStepNrvWait));

    MR::validateCollisionParts(this);

    MR::showModel(this);
    appear();
}

void IceStep::destroy() {
    MR::startBck(this, "End", nullptr);
    MR::startBpk(this, "End");
    MR::startSound(this, "SE_OJ_ICE_FLOOR_MELT");

    mTimer = 0;
}

void IceStep::doHit() {
    MR::invalidateCollisionParts(this);

    setNerve(GET_NERVE(IceStep, IceStepNrvHit));

    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_ICE_FLOOR_BREAK");
    MR::startBck(this, "End", nullptr);

    mTimer = 0;
}

bool IceStep::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool IceStep::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isOnPlayer(this)) {
        return false;
    }

    if (isNerve(GET_NERVE(IceStep, IceStepNrvWait))) {
        doHit();
        return true;
    }

    return false;
}

void IceStep::control() {
}
