#include "Game/Boss/PoltaGroundRock.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvPoltaGroundRock {
    NEW_NERVE(PoltaGroundRockNrvSign, PoltaGroundRock, Sign);
    NEW_NERVE(PoltaGroundRockNrvJut, PoltaGroundRock, Jut);
    NEW_NERVE(PoltaGroundRockNrvWait, PoltaGroundRock, Wait);
    NEW_NERVE(PoltaGroundRockNrvBreak, PoltaGroundRock, Break);
};
PoltaGroundRock::PoltaGroundRock(const char* pName)
    : LiveActor(pName), mOwner(nullptr), mBreakModel(nullptr), _94(0.0f, 0.0f, 0.0f, 1.0f), _A4(0.0f, 0.0f, 0.0f) {}

void PoltaGroundRock::init(const JMapInfoIter&rIter) {
  MR::initDefaultPos(this, rIter);
  initModelManagerWithAnm("PoltaGroundRock", nullptr, false);
  MR::connectToSceneCollisionMapObjWeakLight(this);
  MR::initLightCtrl(this);
  initHitSensor(2);
  MR::addHitSensorMapObj(this, "collision", 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
  MR::addHitSensorEnemy(this, "body", 16, 170.0f, TVec3f(0.0f, 260.0f, 0.0f));
  MR::initCollisionParts(this, "PoltaGroundRock", getSensor("collision"), nullptr);
  initEffectKeeper(0, nullptr, false);
  MR::setEffectHostSRT(this, "Shadow", &_A4, nullptr, nullptr);
  initSound(4, false);
  initNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvSign::sInstance);
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

void PoltaGroundRock::control() {}

void PoltaGroundRock::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _94);
    MR::setBaseScale(this, mScale);
}

void PoltaGroundRock::start(LiveActor* pOwner, const TVec3f& rVec1) {
    mOwner = pOwner;
    setNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvSign::sInstance);
    _A4.set(rVec1);
    MR::calcGravity(this, rVec1);
    JMAVECScaleAdd(&mGravity, &_A4, &mPosition, 500.0f);
    TPos3f v9;
    MR::makeMtxUpNoSupportPos(&v9, mGravity.negateInline(), mPosition);
    MR::rotateMtxLocalY(v9, MR::getRandom(0.0f, 6.2831855f));
    v9.getQuat(_94);
    MR::validateCollisionParts(this);
    MR::showModel(this);
    appear();
}

void PoltaGroundRock::requestBreak() {
    if (PoltaGroundRock::isEnableBreak()) {
        setNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvBreak::sInstance);
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
        setNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvBreak::sInstance);
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
            setNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvBreak::sInstance);
            return true;
        }
    }
    return false;
}

bool PoltaGroundRock::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_BREAK_POLTA_GROUND_ROCK && getSensor("body") == pReceiver && isEnableBreak()) {
        setNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvBreak::sInstance);
        return true;
    }
    return false;
}

void PoltaGroundRock::exeSign() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Shadow");
    }
    MR::startLevelSound(this, "SE_OJ_LV_POLTA_G_ROCK_SIGN", -1, -1, -1);
    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvJut::sInstance);
    }
}

void PoltaGroundRock::exeJut() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_POLTA_G_ROCK_JUT", -1, -1);
    }
    JMAVECScaleAdd(&mGravity, &_A4, &mPosition, MR::calcNerveEaseOutValue(this, 20, 500.0f, 0.0f));
    if (MR::isGreaterStep(this, 20)) {
        MR::tryRumblePadAndCameraDistanceMiddle(this, 800.0f, 1200.0f, 2000.0f);
        setNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvWait::sInstance);
        MR::deleteEffect(this, "Shadow");
    }
}

void PoltaGroundRock::exeWait() {
    if (MR::isFirstStep(this)) {
        mPosition.set(_A4);
        MR::emitEffect(this, "Rock");
    }
}

void PoltaGroundRock::exeBreak() {
    if (MR::isFirstStep(this)) {
        mBreakModel->makeActorAppeared();
        MR::startAction(mBreakModel, "Break");
        MR::deleteEffect(this, "Rock");
        MR::startSound(this, "SE_OJ_POLTA_G_ROCK_BREAK", -1, -1);
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
    return isNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvJut::sInstance);
}

bool PoltaGroundRock::isEnableBreak() const {
    return !isNerve(&NrvPoltaGroundRock::PoltaGroundRockNrvBreak::sInstance);
}
