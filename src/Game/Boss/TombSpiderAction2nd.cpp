#include "Game/Boss/TombSpiderAction2nd.hpp"
#include "Game/Boss/TombSpider.hpp"
#include "Game/Boss/TombSpiderActionBase.hpp"
#include "Game/Boss/TombSpiderFunction.hpp"
#include "Game/Boss/TombSpiderStateSwoon.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

namespace {
    static const s32 sStepWaitMin = 30;
    static const s32 sStepWaitMax = 60;
    static const s32 sStepAttackLoopMin = 240;
    static const s32 sStepAttackLoopMax = 360;
    static const f32 sAttackRotateAccel = 0.005f;
    static const f32 sAttackRotateSpeedMax = 0.4f;
    static const s32 sStepChanceWait = 600;
    static const s32 sChanceDamageLastStep = 5;
};  // namespace

namespace NrvTombSpiderAction2nd {
    NEW_NERVE(TombSpiderAction2ndNrvWait, TombSpiderAction2nd, Wait);
    NEW_NERVE(TombSpiderAction2ndNrvAttackStart, TombSpiderAction2nd, AttackStart);
    NEW_NERVE(TombSpiderAction2ndNrvAttackLoop, TombSpiderAction2nd, AttackLoop);
    NEW_NERVE(TombSpiderAction2ndNrvAttackEnd, TombSpiderAction2nd, AttackEnd);
    NEW_NERVE(TombSpiderAction2ndNrvDamageEye, TombSpiderAction2nd, DamageEye);
    NEW_NERVE(TombSpiderAction2ndNrvDamageHip, TombSpiderAction2nd, DamageHip);
    NEW_NERVE(TombSpiderAction2ndNrvDamageGland, TombSpiderAction2nd, DamageGland);
    NEW_NERVE(TombSpiderAction2ndNrvSwoon, TombSpiderAction2nd, Swoon);
    NEW_NERVE(TombSpiderAction2ndNrvChanceStart, TombSpiderAction2nd, ChanceStart);
    NEW_NERVE(TombSpiderAction2ndNrvChanceWait, TombSpiderAction2nd, ChanceWait);
    NEW_NERVE(TombSpiderAction2ndNrvChanceDamage, TombSpiderAction2nd, ChanceDamage);
    NEW_NERVE(TombSpiderAction2ndNrvChanceDamageLast, TombSpiderAction2nd, ChanceDamageLast);
    NEW_NERVE(TombSpiderAction2ndNrvChanceEnd, TombSpiderAction2nd, ChanceEnd);
    NEW_NERVE(TombSpiderAction2ndNrvWaitDemo, TombSpiderAction2nd, WaitDemo);
};  // namespace NrvTombSpiderAction2nd

TombSpiderAction2nd::TombSpiderAction2nd(TombSpider* pParent) : TombSpiderActionBase(pParent, "戦闘２回戦[トゥームスパイダー]") {
}

void TombSpiderAction2nd::init() {
    initNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvWait));
    MR::initActorStateKeeper(this, 16);
    MR::initActorState(this, new TombSpiderStateSwoon(mParent), GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvSwoon), "Swoon");
    TombSpiderActionBase::init();
}

void TombSpiderAction2nd::appear() {
    TombSpiderActionBase::appear();
    TombSpiderFunction::validateMainPartsSensor(mParent);
    TombSpiderFunction::validateGlandSensor(mParent);
}

void TombSpiderAction2nd::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (TombSpiderFunction::isSpiderAttack(pSender) && isNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvAttackLoop))) {
        if (MR::isSensorPlayer(pReceiver) || TombSpiderFunction::isSpringAttacker(pReceiver)) {
            if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                mHitStep = getNerveStep();
            }
        }
    }
}

bool TombSpiderAction2nd::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SLING_SHOOT_IS_LAST_DAMAGE) {
        return mIsDead;
    }

    if (msg == ACTMES_IS_BROKEN) {
        return !TombSpiderFunction::isSpiderBody(pReceiver);
    }

    if (msg == ACTMES_SLING_SHOOT_ATTACK) {
        if (!isNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvSwoon)) && TombSpiderFunction::tryDamageEye(pSender, pReceiver)) {
            setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvDamageEye));
            return true;
        }

        if (TombSpiderFunction::tryDamageHip(pSender, pReceiver)) {
            setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvDamageHip));
            return true;
        }

        if (TombSpiderFunction::tryDamageGland(mParent, pSender, pReceiver)) {
            setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvDamageGland));
            return true;
        }

        if (isNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceWait)) &&
            TombSpiderFunction::tryDamageVitalSpot(mParent, pSender, pReceiver)) {
            startChanceDamage();
            if (mEnergy <= 0) {
                setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceDamageLast));
            } else {
                setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceDamage));
            }
            return true;
        }

        if (TombSpiderFunction::isSpiderBody(pReceiver)) {
            return true;
        }
    }

    if (msg == ACTMES_SLING_SHOOT_PASS_THROUGH_ENABLE) {
        return isNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceDamage));
    }

    return false;
}

void TombSpiderAction2nd::exeWait() {
    if (updateWait(::sStepWaitMin, ::sStepWaitMax)) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvAttackStart));
    }
}

void TombSpiderAction2nd::exeAttackStart() {
    if (updateAttackStart()) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvAttackLoop));
    }
}

void TombSpiderAction2nd::exeAttackLoop() {
    if (updateAttackLoop(::sStepAttackLoopMin, ::sStepAttackLoopMax, ::sAttackRotateAccel, ::sAttackRotateSpeedMax)) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvAttackEnd));
    }
}

void TombSpiderAction2nd::exeAttackEnd() {
    if (updateAttackEnd()) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvWait));
    }
}

void TombSpiderAction2nd::exeDamageEye() {
    if (updateDamageEye()) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvSwoon));
    }
}

void TombSpiderAction2nd::exeDamageHip() {
    if (updateDamageHip()) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvWaitDemo));
        if (!tryWaitChanceStartDemo(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceStart))) {
            setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceStart));
        }
    }
}

void TombSpiderAction2nd::exeDamageGland() {
    if (updateDamageGland()) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvSwoon));
    }
}

void TombSpiderAction2nd::exeSwoon() {
    MR::updateActorStateAndNextNerve(this, GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvWait));
}

void TombSpiderAction2nd::exeChanceStart() {
    if (updateChanceStart()) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceWait));
    }
}

void TombSpiderAction2nd::exeChanceWait() {
    if (updateChanceWait(::sStepChanceWait)) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvWaitDemo));
        if (!tryWaitChanceEndDemo(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceEnd))) {
            setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceEnd));
        }
    }
}

void TombSpiderAction2nd::exeChanceDamage() {
    if (updateChanceDamage()) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvChanceWait));
    }
}

void TombSpiderAction2nd::exeChanceDamageLast() {
    if (MR::isStep(this, ::sChanceDamageLastStep)) {
        kill();
    }
}

void TombSpiderAction2nd::exeChanceEnd() {
    if (updateChanceEnd()) {
        setNerve(GET_NERVE(TombSpiderAction2nd, TombSpiderAction2ndNrvWait));
    }
}

void TombSpiderAction2nd::exeWaitDemo() {
}
