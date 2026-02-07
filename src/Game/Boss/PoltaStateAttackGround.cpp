#include "Game/Boss/PoltaStateAttackGround.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvPoltaStateAttackGround {
    NEW_NERVE(PoltaStateAttackGroundNrvStart, PoltaStateAttackGround, Start);
    NEW_NERVE(PoltaStateAttackGroundNrvAttack, PoltaStateAttackGround, Attack);
    NEW_NERVE(PoltaStateAttackGroundNrvToWait, PoltaStateAttackGround, ToWait);

};

PoltaStateAttackGround::PoltaStateAttackGround(Polta* pPolta) : ActorStateBase<Polta>("[state]地面叩き攻撃"), mPoltaPtr(pPolta),
mIsAffectBody(true) {
    initNerve(&NrvPoltaStateAttackGround::PoltaStateAttackGroundNrvStart::sInstance);
    mAttackStartLength = MR::getBckFrameMax(mPoltaPtr, "AttackFrontStart");
    mAttackLength = MR::getBckFrameMax(mPoltaPtr, "AttackFront");
    mAttackToWaitLength = MR::getBckFrameMax(mPoltaPtr, "AttackFrontToWait");
};

//Unused remnant also found in PoltaStatePunch?
const char* unusedDamage = "Damage";

void PoltaStateAttackGround::appear() {
    mIsDead = false;
    mIsAffectBody = true;
    setNerve(&NrvPoltaStateAttackGround::PoltaStateAttackGroundNrvStart::sInstance);
}

void PoltaStateAttackGround::exeStart() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::requestStartControllArm(mPoltaPtr);
        PoltaFunction::startAction(mPoltaPtr, "AttackFrontStart", mIsAffectBody);
        MR::startSound(mPoltaPtr, "SE_BV_POLTA_PREP_ARM_DOWN", -1, -1);
        MR::startSound(mPoltaPtr, "SE_BM_POLTA_ARM_LIFT_UP", -1, -1);
    }
    if (MR::isLessStep(this, 60)) {
        mPoltaPtr->rotateToPlayer();
    }
    if (!PoltaFunction::isEnableAttackLeftArm(mPoltaPtr) && !PoltaFunction::isEnableAttackRightArm(mPoltaPtr)) {
        kill();
    } else {
        if (MR::isGreaterStep(this, mAttackStartLength)) {
            setNerve(&NrvPoltaStateAttackGround::PoltaStateAttackGroundNrvAttack::sInstance);
        }
    }
}

void PoltaStateAttackGround::exeAttack() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mPoltaPtr, "AttackFront", mIsAffectBody);
        MR::startSound(mPoltaPtr, "SE_BV_POLTA_ARM_DOWN", -1, -1);
        MR::startSound(mPoltaPtr, "SE_BM_POLTA_ARM_SWING_DOWN", -1, -1);
    }
    if (!PoltaFunction::isEnableAttackLeftArm(mPoltaPtr) && !PoltaFunction::isEnableAttackRightArm(mPoltaPtr)) {
        kill();
    } else {
        if (MR::isGreaterStep(this, mAttackLength)) {
            setNerve(&NrvPoltaStateAttackGround::PoltaStateAttackGroundNrvToWait::sInstance);
            MR::tryRumblePadStrong(this, 0);
            MR::startSound(mPoltaPtr, "SE_BM_POLTA_HIT_GROUND", -1, -1);
            MR::shakeCameraNormalStrong();
        }
    }
}

void PoltaStateAttackGround::exeToWait() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mPoltaPtr, "AttackFrontToWait", mIsAffectBody);
    }
    if (MR::isGreaterStep(this, mAttackToWaitLength)) {
        kill();
    }
}

bool PoltaStateAttackGround::isEnableAttack(const HitSensor* pSensor) const {
    if (PoltaFunction::isBodySensor(mPoltaPtr, pSensor)) {
        return false;
    } else {
        return isNerve(&NrvPoltaStateAttackGround::PoltaStateAttackGroundNrvAttack::sInstance);
    }
}

PoltaStateAttackGround::~PoltaStateAttackGround() {}
