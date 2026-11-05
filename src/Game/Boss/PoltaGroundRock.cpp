#include "Game/Boss/PoltaGroundRock.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

namespace {
    static const s32 sSignTime = 60;
    static const s32 sJutTime = 20;
    static const f32 sStartPosPffset = 500.0f;
};  // namespace

namespace NrvPoltaGroundRock {
    NEW_NERVE(PoltaGroundRockNrvSign, PoltaGroundRock, Sign);
    NEW_NERVE(PoltaGroundRockNrvJut, PoltaGroundRock, Jut);
    NEW_NERVE(PoltaGroundRockNrvWait, PoltaGroundRock, Wait);
    NEW_NERVE(PoltaGroundRockNrvBreak, PoltaGroundRock, Break);
};  // namespace NrvPoltaGroundRock

PoltaGroundRock::PoltaGroundRock(const char* pName)
    : LiveActor(pName), mHost(), mBreakModel(), mQuat(0.0f, 0.0f, 0.0f, 1.0f), mStartPosition(0.0f, 0.0f, 0.0f) {
}

void PoltaGroundRock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("PoltaGroundRock", nullptr, false);
    MR::connectToSceneCollisionMapObjWeakLight(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "collision", 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemy(this, "body", 16, 170.0f, TVec3f(0.0f, 260.0f, 0.0f));
    MR::initCollisionParts(this, "PoltaGroundRock", getSensor("collision"), nullptr);
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostSRT(this, "Shadow", &mStartPosition, nullptr, nullptr);
    initSound(4, false);
    initNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvSign));
    initBreakModel();
    MR::invalidateClipping(this);
    makeActorDead();
}

void PoltaGroundRock::initBreakModel() {
    mBreakModel = MR::createModelObjEnemy("壊れモデル", "PoltaGroundRockBreak", getBaseMtx());
    mBreakModel->initWithoutIter();
    MR::initLightCtrl(mBreakModel);
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
}

void PoltaGroundRock::control() {
}

void PoltaGroundRock::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mQuat);
    MR::setBaseScale(this, mScale);
}

void PoltaGroundRock::start(LiveActor* pHost, const TVec3f& rStartPosition) {
    mHost = pHost;
    setNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvSign));
    mStartPosition.set(rStartPosition);
    MR::calcGravity(this, rStartPosition);
    mPosition.scaleAdd(::sStartPosPffset, mGravity, mStartPosition);
    TPos3f quat;
    MR::makeMtxUpNoSupportPos(&quat, -mGravity, mPosition);
    MR::rotateMtxLocalY(quat, MR::getRandom(0.0f, TWO_PI));
    quat.getQuat(mQuat);
    MR::validateCollisionParts(this);
    MR::showModel(this);
    appear();
}

void PoltaGroundRock::requestBreak() {
    if (PoltaGroundRock::isEnableBreak()) {
        setNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvBreak));
    }
}

void PoltaGroundRock::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableAttack()) {
        if (MR::isSensorPlayer(pReceiver)) {
            MR::sendMsgEnemyAttackStrong(pReceiver, pSender);
        } else {
            MR::sendArbitraryMsg(ACTMES_BREAK_POLTA_GROUND_ROCK, pReceiver, pSender);
        }
    }
}

bool PoltaGroundRock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getSensor("collision") != pReceiver) {
        return false;
    }
    if (MR::isMsgPlayerSpinAttack(msg) && isEnableBreak()) {
        setNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvBreak));
        return true;
    }

    return false;
}

bool PoltaGroundRock::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getSensor("collision") != pReceiver) {
        return false;
    }
    if (MR::isMsgEnemyAttack(msg) || MR::isMsgExplosionAttack(msg)) {
        if (isEnableBreak()) {
            setNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvBreak));
            return true;
        }
    }
    return false;
}

bool PoltaGroundRock::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_BREAK_POLTA_GROUND_ROCK && getSensor("body") == pReceiver && isEnableBreak()) {
        setNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvBreak));
        return true;
    }
    return false;
}

void PoltaGroundRock::exeSign() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Shadow");
    }
    MR::startLevelSound(this, "SE_OJ_LV_POLTA_G_ROCK_SIGN");
    if (MR::isGreaterStep(this, ::sSignTime)) {
        setNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvJut));
    }
}

void PoltaGroundRock::exeJut() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_POLTA_G_ROCK_JUT");
    }
    mPosition.scaleAdd(MR::calcNerveEaseOutValue(this, 20, ::sStartPosPffset, 0.0f), mGravity, mStartPosition);
    if (MR::isGreaterStep(this, ::sJutTime)) {
        MR::tryRumblePadAndCameraDistanceMiddle(this, 800.0f, 1200.0f, 2000.0f);
        setNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvWait));
        MR::deleteEffect(this, "Shadow");
    }
}

void PoltaGroundRock::exeWait() {
    if (MR::isFirstStep(this)) {
        mPosition.set(mStartPosition);
        MR::emitEffect(this, "Rock");
    }
}

void PoltaGroundRock::exeBreak() {
    if (MR::isFirstStep(this)) {
        mBreakModel->makeActorAppeared();
        MR::startAction(mBreakModel, "Break");
        MR::deleteEffect(this, "Rock");
        MR::startSound(this, "SE_OJ_POLTA_G_ROCK_BREAK");
        MR::hideModel(this);
        MR::invalidateCollisionParts(this);
        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
    }
    if (MR::isActionEnd(mBreakModel)) {
        kill();
        mBreakModel->kill();
    }
}

bool PoltaGroundRock::isEnableAttack() const {
    return isNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvJut));
}

bool PoltaGroundRock::isEnableBreak() const {
    return !isNerve(GET_NERVE(PoltaGroundRock, PoltaGroundRockNrvBreak));
}
