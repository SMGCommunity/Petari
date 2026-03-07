#include "Game/Boss/PoltaStatePunch.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvPoltaStatePunch {
    NEW_NERVE(PoltaStatePunchNrvStart, PoltaStatePunch, Start);
    NEW_NERVE(PoltaStatePunchNrvAttack, PoltaStatePunch, Attack);
    NEW_NERVE(PoltaStatePunchNrvToWait, PoltaStatePunch, ToWait);
};  // namespace NrvPoltaStatePunch

PoltaStatePunch::PoltaStatePunch(Polta* pPolta)
    : ActorStateBase< Polta >("[state]地面叩き攻撃", pPolta), mIsLeftArmActor(true), mIsActionAffectBody(true) {
    initNerve(&NrvPoltaStatePunch::PoltaStatePunchNrvStart::sInstance);
}

void PoltaStatePunch::appear() {
    mIsActionAffectBody = true;
    mIsDead = false;
    setNerve(&NrvPoltaStatePunch::PoltaStatePunchNrvStart::sInstance);
}

PoltaArm* PoltaStatePunch::getAttackActor() {
    if (mIsLeftArmActor) {
        return PoltaFunction::getLeftArmActor(getHost());
    } else {
        return PoltaFunction::getRightArmActor(getHost());
    }
}

// Unused remnant also found in PoltaStateAttackGround?
const char* unusedDamagePunch = "Damage";

void PoltaStatePunch::exeStart() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::requestStartControllArm(getHost());
        PoltaFunction::startAction(getHost(), mIsLeftArmActor ? "PunchLeftStart" : "PunchRightStart", mIsActionAffectBody);
        MR::startSound(getHost(), "SE_BV_POLTA_PREP_PUNCH", -1, -1);
    }
    if (MR::isLessStep(this, 60)) {
        getHost()->rotateToPlayer();
    }

    if (!isEnablePunchArm()) {
        kill();
    } else {
        if (MR::isActionEnd(getAttackActor())) {
            setNerve(&NrvPoltaStatePunch::PoltaStatePunchNrvAttack::sInstance);
        }
    }
}

void PoltaStatePunch::exeAttack() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), mIsLeftArmActor ? "PunchLeft" : "PunchRight", mIsActionAffectBody);
        MR::startSound(getHost(), "SE_BV_POLTA_PUNCH", -1, -1);
        MR::startSound(getHost(), "SE_BM_POLTA_PUNCH", -1, -1);
    }

    if (!isEnablePunchArm()) {
        kill();
    } else {
        if (MR::isActionEnd(getAttackActor())) {
            setNerve(&NrvPoltaStatePunch::PoltaStatePunchNrvToWait::sInstance);
        }
    }
}

void PoltaStatePunch::exeToWait() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), mIsLeftArmActor ? "PunchLeftToWait" : "PunchRightToWait", mIsActionAffectBody);
    }

    if (!isEnablePunchArm()) {
        kill();
        return;
    }
    if (MR::isGreaterStep(this, 70)) {
        getHost()->rotateToPlayer();
    }
    if (MR::isActionEnd(getAttackActor())) {
        kill();
    }
}

bool PoltaStatePunch::isEnableAttack(const HitSensor* pSensor) const {
    if (!NerveExecutor::isNerve(&NrvPoltaStatePunch::PoltaStatePunchNrvAttack::sInstance)) {
        return false;
    }

    return mIsLeftArmActor ? PoltaFunction::isLeftArmSensor(this->getHost(), pSensor) : PoltaFunction::isRightArmSensor(this->getHost(), pSensor);
}

bool PoltaStatePunch::isEnablePunchArm() const {
    return mIsLeftArmActor ? PoltaFunction::isEnableAttackLeftArm(getHost()) : PoltaFunction::isEnableAttackRightArm(getHost());
}

PoltaStatePunch::~PoltaStatePunch() {}
