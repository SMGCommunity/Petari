#include "Game/Boss/DinoPackunBattleVs1Lv2.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunStateDamage.hpp"
#include "JSystem/JMath/JMath.hpp"

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
    initNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvStart::sInstance);
    mStateDamage = new DinoPackunStateDamage(pPackun);
    mStateDamage->setDamageNormal();
}

void DinoPackunBattleVs1Lv2::appear() {
    if (MR::isDead(getHost())) {
        getHost()->makeActorAppeared();
    }

    mIsDead = false;

    if (_18) {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvStart::sInstance);
    } else {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvFind::sInstance);
    }
}

void DinoPackunBattleVs1Lv2::attackSensor(HitSensor* a1, HitSensor* a2) {
    if (MR::isSensorPlayer(a2)) {
        bool v6 = false;

        if (isNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvChase::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvWalk::sInstance)) {
            v6 = true;
        }

        if (v6 && sendBlowAttackMessage(a1, a2, false)) {
            setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvAttackHit::sInstance);
            return;
        }

        bool v7 = false;

        if (isNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvTurn::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvFind::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvCoolDown::sInstance)) {
            v7 = true;
        }

        if (v7 && sendHitAttackMessage(a1, a2, false)) {
            setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvAttackHit::sInstance);
            return;
        }

        MR::sendMsgPush(a2, a1);
    } else {
        bool v8 = false;

        if (isNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvChase::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvWalk::sInstance)) {
            v8 = true;
        }

        if (v8) {
            MR::sendMsgEnemyAttack(a2, a1);
        }
    }
}

bool DinoPackunBattleVs1Lv2::receiveMsgPlayerAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        getHost()->startHitReaction();
        return true;
    } else if (MR::isMsgPlayerSpinAttack(msg) && MR::sendMsgEnemyAttackFlipWeakJump(a2, a3)) {
        MR::emitEffectHitBetweenSensors(getHost(), a2, a3, 0.0f, "InvalidHitMark");
        return false;
    }

    return false;
}

bool DinoPackunBattleVs1Lv2::receiveOtherMsg(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (isNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvDamage::sInstance)) {
        return mStateDamage->receiveOtherMsg(msg, a2, a3);
    }

    if (mStateDamage->isDamageMessage(msg)) {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvDamage::sInstance);
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
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvFind::sInstance);
        return true;
    }

    return false;
}

void DinoPackunBattleVs1Lv2::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Find", nullptr);
    }

    if (updateStart()) {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvChase::sInstance);
    }
}

void DinoPackunBattleVs1Lv2::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "AngryWalk");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_WALK", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    TVec3f side;
    MR::calcSideVec(&side, getHost());
    TVec3f v7;
    v7.setPS(side);
    v7.x *= _10;
    v7.y *= _10;
    v7.z *= _10;
    MR::turnDirectionDegree(getHost(), &getHost()->_E8, v7, 1.0f);
    MR::addVelocityMoveToDirection(getHost(), getHost()->_E8, 1.3f);
    getHost()->updateRunVelocity();
    getHost()->updateFootPrintNerve(getNerveStep(), 50);

    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvWalk::sInstance);
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
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvTurn::sInstance);
    } else {
        if (tryFind()) {
            return;
        }
    }
}

void DinoPackunBattleVs1Lv2::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Chase", nullptr);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_CHASE", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    bool isHit = getHost()->isHitReaction(15);
    f32 v3 = isHit ? 0.0f : 1.5f;
    f32 v4 = isHit ? 0.0f : 1.0f;

    if (updateChase(300, 180.0f, v4, v3, 90, 46)) {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvCoolDown::sInstance);
    }
}

void DinoPackunBattleVs1Lv2::exeDamage() {
    if (MR::updateActorState(this, mStateDamage)) {
        kill();
    }
}

void DinoPackunBattleVs1Lv2::exeAttackHit() {
    if (updateAttackHit()) {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvTurn::sInstance);
    }
}

void DinoPackunBattleVs1Lv2::exeCoolDown() {
    if (updateCoolDown(0x1E)) {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvTurn::sInstance);
    }
}

void DinoPackunBattleVs1Lv2::exeFind() {
    if (updateFind(0xE, 1.0f)) {
        setNerve(&NrvDinoPackunBattleVs1Lv2::DinoPackunBattleVs1Lv2NrvChase::sInstance);
    }
}

DinoPackunBattleVs1Lv2::~DinoPackunBattleVs1Lv2() {
    return;
}
