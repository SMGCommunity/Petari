#include "Game/Boss/DinoPackunBattleVs1Lv2.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunStateDamage.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvDinoPackunBattleVs1Lv2 {
    NEW_NERVE(DinoPackunBattleVs1Lv2NrvStart, DinoPackunBattleVs1Lv2, Start);
    NEW_NERVE(DinoPackunBattleVs1Lv2NrvTurn, DinoPackunBattleVs1Lv2, Turn);
    NEW_NERVE(DinoPackunBattleVs1Lv2NrvWalk, DinoPackunBattleVs1Lv2, Walk);
    NEW_NERVE(DinoPackunBattleVs1Lv2NrvFind, DinoPackunBattleVs1Lv2, Find);
    NEW_NERVE(DinoPackunBattleVs1Lv2NrvChase, DinoPackunBattleVs1Lv2, Chase);
    NEW_NERVE(DinoPackunBattleVs1Lv2NrvCoolDown, DinoPackunBattleVs1Lv2, CoolDown);
    NEW_NERVE(DinoPackunBattleVs1Lv2NrvAttackHit, DinoPackunBattleVs1Lv2, AttackHit);
    NEW_NERVE(DinoPackunBattleVs1Lv2NrvDamage, DinoPackunBattleVs1Lv2, Damage);
};  // namespace NrvDinoPackunBattleVs1Lv2

DinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2(DinoPackun* pPackun) : DinoPackunAction("ラス", pPackun) {
    mStateDamage = nullptr;
    _18 = 1;
    initNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvStart));
    mStateDamage = new DinoPackunStateDamage(pPackun);
    mStateDamage->setDamageNormal();
}

void DinoPackunBattleVs1Lv2::appear() {
    if (MR::isDead(getHost())) {
        getHost()->makeActorAppeared();
    }

    mIsDead = false;

    if (_18) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvStart));
    } else {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvFind));
    }
}

void DinoPackunBattleVs1Lv2::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        bool v6 = isNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvChase)) ||
                  isNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvWalk));

        if (v6 && sendBlowAttackMessage(pSender, pReceiver, false)) {
            setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvAttackHit));
            return;
        }

        bool v7 = isNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvTurn)) ||
                  isNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvFind)) ||
                  isNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvCoolDown));

        if (v7 && sendHitAttackMessage(pSender, pReceiver, false)) {
            setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvAttackHit));
            return;
        }

        MR::sendMsgPush(pReceiver, pSender);
    } else {
        bool v8 = isNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvChase)) ||
                  isNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvWalk));

        if (v8) {
            MR::sendMsgEnemyAttack(pReceiver, pSender);
        }
    }
}

bool DinoPackunBattleVs1Lv2::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        getHost()->startHitReaction();
        return true;
    } else if (MR::isMsgPlayerSpinAttack(msg) && MR::sendMsgEnemyAttackFlipWeakJump(pSender, pReceiver)) {
        MR::emitEffectHitBetweenSensors(getHost(), pSender, pReceiver, 0.0f, "InvalidHitMark");
        return false;
    }

    return false;
}

bool DinoPackunBattleVs1Lv2::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvDamage))) {
        return mStateDamage->receiveOtherMsg(msg, pSender, pReceiver);
    }

    if (mStateDamage->isDamageMessage(msg)) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvDamage));
        return true;
    }

    return false;
}

void DinoPackunBattleVs1Lv2::setMiddleBattle() {
    mStateDamage->setDamageNormal();
}

void DinoPackunBattleVs1Lv2::setLastBattle() {
    mStateDamage->setDamageLast();
}

void DinoPackunBattleVs1Lv2::setStartFromDemo() {
    _18 = true;
}

void DinoPackunBattleVs1Lv2::setStartContinueBattle() {
    _18 = false;
}

bool DinoPackunBattleVs1Lv2::tryFind() {
    if (MR::isInSightConePlayer(getHost(), getHost()->_E8, 1000.0f, 80.0f)) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvFind));
        return true;
    }

    return false;
}

void DinoPackunBattleVs1Lv2::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Find", nullptr);
    }

    if (updateStart()) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvChase));
    }
}

void DinoPackunBattleVs1Lv2::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "AngryWalk");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_WALK");
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER");
    }

    TVec3f side;
    MR::calcSideVec(&side, getHost());
    MR::turnDirectionDegree(getHost(), &getHost()->_E8, side * _10, 1.0f);
    MR::addVelocityMoveToDirection(getHost(), getHost()->_E8, 1.3f);
    getHost()->updateRunVelocity();
    getHost()->updateFootPrintNerve(getNerveStep(), 50);

    if (MR::isGreaterStep(this, 30)) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvWalk));
    } else {
        if (tryFind()) {
            return;
        }
    }
}

void DinoPackunBattleVs1Lv2::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "AngryWalk");
    }

    if (updateWalk(180, 0.5f, 50)) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvTurn));
    } else {
        if (tryFind()) {
            return;
        }
    }
}

void DinoPackunBattleVs1Lv2::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Chase", nullptr);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_CHASE");
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER");
    }

    bool isHit = getHost()->isHitReaction(15);
    f32 v3 = isHit ? 0.0f : 1.5f;
    f32 v4 = isHit ? 0.0f : 1.0f;

    if (updateChase(300, 180.0f, v4, v3, 90, 46)) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvCoolDown));
    }
}

void DinoPackunBattleVs1Lv2::exeDamage() {
    if (MR::updateActorState(this, mStateDamage)) {
        kill();
    }
}

void DinoPackunBattleVs1Lv2::exeAttackHit() {
    if (updateAttackHit()) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvTurn));
    }
}

void DinoPackunBattleVs1Lv2::exeCoolDown() {
    if (updateCoolDown(0x1E)) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvTurn));
    }
}

void DinoPackunBattleVs1Lv2::exeFind() {
    if (updateFind(0xE, 1.0f)) {
        setNerve(GET_NERVE(DinoPackunBattleVs1Lv2, DinoPackunBattleVs1Lv2NrvChase));
    }
}

DinoPackunBattleVs1Lv2::~DinoPackunBattleVs1Lv2() {
    return;
}
