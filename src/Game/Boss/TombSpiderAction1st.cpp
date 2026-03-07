#include "Game/Boss/TombSpiderAction1st.hpp"
#include "Game/Boss/TombSpider.hpp"
#include "Game/Boss/TombSpiderActionBase.hpp"
#include "Game/Boss/TombSpiderFunction.hpp"
#include "Game/Boss/TombSpiderStateSwoon.hpp"

namespace {
    static const s32 sStepWaitMin = 60;
    static const s32 sStepWaitMax = 90;
    static const s32 sStepAttackLoopMin = 180;
    static const s32 sStepAttackLoopMax = 300;
    static const f32 sAttackRotateAccel = 0.01f;
    static const f32 sAttackRotateSpeedMax = 0.35f;
    static const s32 sStepChanceWait = 900;
    static const s32 sChanceDamageLastStep = 5;
}  // namespace

namespace NrvTombSpiderAction1st {
    NEW_NERVE(TombSpiderAction1stNrvWait, TombSpiderAction1st, Wait);
    NEW_NERVE(TombSpiderAction1stNrvAttackStart, TombSpiderAction1st, AttackStart);
    NEW_NERVE(TombSpiderAction1stNrvAttackLoop, TombSpiderAction1st, AttackLoop);
    NEW_NERVE(TombSpiderAction1stNrvAttackEnd, TombSpiderAction1st, AttackEnd);
    NEW_NERVE(TombSpiderAction1stNrvDamageEye, TombSpiderAction1st, DamageEye);
    NEW_NERVE(TombSpiderAction1stNrvDamageHip, TombSpiderAction1st, DamageHip);
    NEW_NERVE(TombSpiderAction1stNrvDamageGland, TombSpiderAction1st, DamageGland);
    NEW_NERVE(TombSpiderAction1stNrvSwoon, TombSpiderAction1st, Swoon);
    NEW_NERVE(TombSpiderAction1stNrvChanceStart, TombSpiderAction1st, ChanceStart);
    NEW_NERVE(TombSpiderAction1stNrvChanceWait, TombSpiderAction1st, ChanceWait);
    NEW_NERVE(TombSpiderAction1stNrvChanceDamage, TombSpiderAction1st, ChanceDamage);
    NEW_NERVE(TombSpiderAction1stNrvChanceDamageLast, TombSpiderAction1st, ChanceDamageLast);
    NEW_NERVE(TombSpiderAction1stNrvChanceEnd, TombSpiderAction1st, ChanceEnd);
    NEW_NERVE(TombSpiderAction1stNrvWaitDemo, TombSpiderAction1st, WaitDemo);
}  // namespace NrvTombSpiderAction1st

TombSpiderAction1st::TombSpiderAction1st(TombSpider* pParent) : TombSpiderActionBase(pParent, "戦闘１回戦[トゥームスパイダー]") {
}

void TombSpiderAction1st::init() {
    initNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvWait::sInstance);
    MR::initActorStateKeeper(this, 16);
    MR::initActorState(this, new TombSpiderStateSwoon(mParent), &NrvTombSpiderAction1st::TombSpiderAction1stNrvSwoon::sInstance, "Swoon");
    TombSpiderActionBase::init();
}

void TombSpiderAction1st::appear() {
    TombSpiderActionBase::appear();
    TombSpiderFunction::validateMainPartsSensor(mParent);
    TombSpiderFunction::validateGlandSensor(mParent);
}

void TombSpiderAction1st::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (TombSpiderFunction::isEnemyAttack(pSender) && isNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvAttackLoop::sInstance)) {
        if (MR::isSensorPlayer(pReceiver) || TombSpiderFunction::isSpringAttacker(pReceiver)) {
            if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                mHitStep = getNerveStep();
            }
        }
    }
}

bool TombSpiderAction1st::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SLING_SHOOT_IS_LAST_DAMAGE) {
        return mIsDead;
    }

    if (msg == ACTMES_IS_BROKEN) {
        return !TombSpiderFunction::isSpiderBody(pReceiver);
    }

    if (msg == ACTMES_SLING_SHOOT_ATTACK) {
        if (!isNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvSwoon::sInstance) && TombSpiderFunction::tryDamageEye(pSender, pReceiver)) {
            setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvDamageEye::sInstance);
            return true;
        }

        if (TombSpiderFunction::tryDamageHip(pSender, pReceiver)) {
            setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvDamageHip::sInstance);
            return true;
        }

        if (TombSpiderFunction::tryDamageGland(mParent, pSender, pReceiver)) {
            setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvDamageGland::sInstance);
            return true;
        }

        if (isNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceWait::sInstance) &&
            TombSpiderFunction::tryDamageVitalSpot(mParent, pSender, pReceiver)) {
            startChanceDamage();
            if (mEnergy <= 0) {
                setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceDamageLast::sInstance);
            } else {
                setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceDamage::sInstance);
            }
            return true;
        }

        if (TombSpiderFunction::isSpiderBody(pReceiver)) {
            return true;
        }
    }

    if (msg == ACTMES_SLING_SHOOT_PASS_THROUGH_ENABLE) {
        return isNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceDamage::sInstance);
    }

    return false;
}

void TombSpiderAction1st::exeWait() {
    if (updateWait(sStepWaitMin, sStepWaitMax)) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvAttackStart::sInstance);
    }
}

void TombSpiderAction1st::exeAttackStart() {
    if (updateAttackStart()) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvAttackLoop::sInstance);
    }
}

void TombSpiderAction1st::exeAttackLoop() {
    if (updateAttackLoop(sStepAttackLoopMin, sStepAttackLoopMax, sAttackRotateAccel, sAttackRotateSpeedMax)) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvAttackEnd::sInstance);
    }
}

void TombSpiderAction1st::exeAttackEnd() {
    if (updateAttackEnd()) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvWait::sInstance);
    }
}

void TombSpiderAction1st::exeDamageEye() {
    if (updateDamageEye()) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvSwoon::sInstance);
    }
}

void TombSpiderAction1st::exeDamageHip() {
    if (updateDamageHip()) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvWaitDemo::sInstance);
        if (!tryWaitChanceStartDemo(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceStart::sInstance)) {
            setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceStart::sInstance);
        }
    }
}

void TombSpiderAction1st::exeDamageGland() {
    if (updateDamageGland()) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvSwoon::sInstance);
    }
}

void TombSpiderAction1st::exeSwoon() {
    MR::updateActorStateAndNextNerve(this, &NrvTombSpiderAction1st::TombSpiderAction1stNrvWait::sInstance);
}

void TombSpiderAction1st::exeChanceStart() {
    if (updateChanceStart()) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceWait::sInstance);
    }
}

void TombSpiderAction1st::exeChanceWait() {
    if (updateChanceWait(sStepChanceWait)) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvWaitDemo::sInstance);
        if (!tryWaitChanceEndDemo(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceEnd::sInstance)) {
            setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceEnd::sInstance);
        }
    }
}

void TombSpiderAction1st::exeChanceDamage() {
    if (updateChanceDamage()) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvChanceWait::sInstance);
    }
}

void TombSpiderAction1st::exeChanceDamageLast() {
    if (MR::isStep(this, sChanceDamageLastStep)) {
        kill();
    }
}

void TombSpiderAction1st::exeChanceEnd() {
    if (updateChanceEnd()) {
        setNerve(&NrvTombSpiderAction1st::TombSpiderAction1stNrvWait::sInstance);
    }
}

void TombSpiderAction1st::exeWaitDemo() {
}
