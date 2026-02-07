#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/ScreenUtil.hpp"

PoltaActionBase::PoltaActionBase(const char* pName, Polta* pPolta) : ActorStateBase<Polta>(pName), mPoltaPtr(pPolta), _10(0) {
    MR::createCenterScreenBlur();
}

void PoltaActionBase::updateScreamSensor() {
    if (MR::isLessStep(this, 30)) {
        PoltaFunction::setScreamSensorSize(mPoltaPtr, MR::calcNerveValue(this, 30, 0.0f, 3200.0f));
    } else {
        PoltaFunction::setScreamSensorSize(mPoltaPtr, 0.0f);
    }
}

PoltaActionBase::~PoltaActionBase() {}

bool PoltaActionBase::updateWait() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mPoltaPtr, "Wait", true);
    }
    MR::startLevelSound(mPoltaPtr, "SE_BM_LV_POLTA_IN_BATTLE_ROCK", -1, -1, -1);
    mPoltaPtr->rotateToPlayer();
    PoltaFunction::requestStartControllArm(mPoltaPtr);
    return false;
}

bool PoltaActionBase::updateDamageBody(bool isFirst) {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mPoltaPtr, "Damage", true);
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1", -1, -1);
        MR::startSound(mPoltaPtr, "SE_BM_POLTA_ROCK_DAMAGE", -1, -1);
        MR::startSound(mPoltaPtr, "SE_BV_POLTA_DAMAGE_BODY", -1, -1);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormalStrong();
        MR::stopScene(3);
        mPoltaPtr->_E4 = 0.0f;
        if (isFirst) {
            mPoltaPtr->appearBreakModelFirst(mPoltaPtr->mPosition);
        } else {
            mPoltaPtr->appearBreakModelMiddle(mPoltaPtr->mPosition);
        }
    }
    MR::startLevelSound(mPoltaPtr, "SE_BM_LV_POLTA_IN_BATTLE_ROCK", -1, -1, -1);
    if (mPoltaPtr->isEndBreakModel()) {
        mPoltaPtr->killBreakModel();
    }
    if (MR::isActionEnd(mPoltaPtr)) {
        mPoltaPtr->killBreakModel();
        PoltaFunction::setScreamSensorSize(mPoltaPtr, 0.0f);
        return true;
    }
    return false;
}

bool PoltaActionBase::updateBreakBody() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mPoltaPtr, "BreakBody", true);
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1", -1, -1);
        MR::startSound(mPoltaPtr, "SE_BM_POLTA_ROCK_BREAK", -1, -1);
        MR::startSound(mPoltaPtr, "SE_BV_POLTA_LOSE_SHELL", -1, -1);
        PoltaFunction::breakLeftArm(mPoltaPtr);
        PoltaFunction::breakRightArm(mPoltaPtr);
        PoltaFunction::killPoltaRock(mPoltaPtr);
        PoltaFunction::breakGroundRock(mPoltaPtr);
        MR::tryRumblePadStrong(this, 0);
        MR::zeroVelocity(mPoltaPtr);
        MR::shakeCameraNormalStrong();
        MR::stopScene(3);
        mPoltaPtr->_E4 = 0.0f;
        mPoltaPtr->appearBreakModelLast(mPoltaPtr->mPosition);
    }
    this->updateScreamSensor();
    if (mPoltaPtr->isEndBreakModel()) {
        mPoltaPtr->killBreakModel();
    }
    if (MR::isActionEnd(mPoltaPtr)) {
        mPoltaPtr->killBreakModel();
        PoltaFunction::setScreamSensorSize(mPoltaPtr, 0.0f);
        return true;
    }
    return false;
}

//Remnants from an unused attack
const char* sinkUnused =  "Sink";
const char* popSignUnsued = "PopSign";
const char* poltaShoutUnused = "SE_BV_POLTA_SHOUT";
const char* screamUnused = "Scream";
const char* repairUnusued = "Repair";

bool PoltaActionBase::updateDamageCore() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mPoltaPtr, "DamageCore", 1);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormalStrong();
        MR::stopScene(3);
        mPoltaPtr->_E4 = 0.0f;
        MR::zeroVelocity(mPoltaPtr);
        PoltaFunction::disperseBombTeresa(mPoltaPtr);
    }
    if (MR::isActionEnd(mPoltaPtr)) {
        MR::startSound(mPoltaPtr, "SE_BM_POLTA_CORE_DOWN", -1, -1);
        PoltaFunction::killBombTeresa(mPoltaPtr);
        MR::zeroVelocity(mPoltaPtr);
        return true;    
    }
    return false;
}

bool PoltaActionBase::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool PoltaActionBase::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool PoltaActionBase::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

void PoltaActionBase::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {}
