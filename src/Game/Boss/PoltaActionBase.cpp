#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

PoltaActionBase::PoltaActionBase(const char* pName, Polta* pPolta) : ActorStateBase< Polta >(pName, pPolta), _10(0) {
    MR::createCenterScreenBlur();
}

void PoltaActionBase::updateScreamSensor() {
    if (MR::isLessStep(this, 30)) {
        f32 size = MR::calcNerveValue(this, 30, 0.0f, 3200.0f);
        PoltaFunction::setScreamSensorSize(getHost(), size);
    } else {
        PoltaFunction::setScreamSensorSize(getHost(), 0.0f);
    }
}

PoltaActionBase::~PoltaActionBase() {
}

bool PoltaActionBase::updateWait() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "Wait", true);
    }
    MR::startLevelSound(getHost(), "SE_BM_LV_POLTA_IN_BATTLE_ROCK");
    getHost()->rotateToPlayer();
    PoltaFunction::requestStartControllArm(getHost());
    return false;
}

bool PoltaActionBase::updateDamageBody(bool isFirst) {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "Damage", true);
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1");
        MR::startSound(getHost(), "SE_BM_POLTA_ROCK_DAMAGE");
        MR::startSound(getHost(), "SE_BV_POLTA_DAMAGE_BODY");
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormalStrong();
        MR::stopScene(3);
        getHost()->_EC = 0.0f;
        if (isFirst) {
            getHost()->appearBreakModelFirst(getHost()->mPosition);
        } else {
            getHost()->appearBreakModelMiddle(getHost()->mPosition);
        }
    }
    MR::startLevelSound(getHost(), "SE_BM_LV_POLTA_IN_BATTLE_ROCK");
    if (getHost()->isEndBreakModel()) {
        getHost()->killBreakModel();
    }
    if (MR::isActionEnd(getHost())) {
        getHost()->killBreakModel();
        PoltaFunction::setScreamSensorSize(getHost(), 0.0f);
        return true;
    }
    return false;
}

bool PoltaActionBase::updateBreakBody() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "BreakBody", true);
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1");
        MR::startSound(getHost(), "SE_BM_POLTA_ROCK_BREAK");
        MR::startSound(getHost(), "SE_BV_POLTA_LOSE_SHELL");
        PoltaFunction::breakLeftArm(getHost());
        PoltaFunction::breakRightArm(getHost());
        PoltaFunction::killPoltaRock(getHost());
        PoltaFunction::breakGroundRock(getHost());
        MR::tryRumblePadStrong(this, 0);
        MR::zeroVelocity(getHost());
        MR::shakeCameraNormalStrong();
        MR::stopScene(3);
        getHost()->_EC = 0.0f;
        getHost()->appearBreakModelLast(getHost()->mPosition);
    }
    this->updateScreamSensor();
    if (getHost()->isEndBreakModel()) {
        getHost()->killBreakModel();
    }
    if (MR::isActionEnd(getHost())) {
        getHost()->killBreakModel();
        PoltaFunction::setScreamSensorSize(getHost(), 0.0f);
        return true;
    }
    return false;
}

// Remnants from an unused attack
const char* sinkUnused = "Sink";
const char* popSignUnsued = "PopSign";
const char* poltaShoutUnused = "SE_BV_POLTA_SHOUT";
const char* screamUnused = "Scream";
const char* repairUnusued = "Repair";

bool PoltaActionBase::updateDamageCore() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "DamageCore", 1);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormalStrong();
        MR::stopScene(3);
        getHost()->_EC = 0.0f;
        MR::zeroVelocity(getHost());
        PoltaFunction::disperseBombTeresa(getHost());
    }
    if (MR::isActionEnd(getHost())) {
        MR::startSound(getHost(), "SE_BM_POLTA_CORE_DOWN");
        PoltaFunction::killBombTeresa(getHost());
        MR::zeroVelocity(getHost());
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

void PoltaActionBase::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
}
