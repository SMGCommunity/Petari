#include "Game/Boss/DinoPackunBattleVs1Lv1.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunStateDamage.hpp"
#include "Game/Util.hpp"

namespace NrvDinoPackunBattleVs1Lv1 {
    NEW_NERVE(DinoPackunBattleVs1Lv1NrvStart, DinoPackunBattleVs1Lv1, Start);
    NEW_NERVE(DinoPackunBattleVs1Lv1NrvTurn, DinoPackunBattleVs1Lv1, Turn);
    NEW_NERVE(DinoPackunBattleVs1Lv1NrvWalk, DinoPackunBattleVs1Lv1, Walk);
    NEW_NERVE(DinoPackunBattleVs1Lv1NrvFind, DinoPackunBattleVs1Lv1, Find);
    NEW_NERVE(DinoPackunBattleVs1Lv1NrvChase, DinoPackunBattleVs1Lv1, Chase);
    NEW_NERVE(DinoPackunBattleVs1Lv1NrvCoolDown, DinoPackunBattleVs1Lv1, CoolDown);
    NEW_NERVE(DinoPackunBattleVs1Lv1NrvAttackHit, DinoPackunBattleVs1Lv1, AttackHit);
    NEW_NERVE(DinoPackunBattleVs1Lv1NrvDamage, DinoPackunBattleVs1Lv1, Damage);
};  // namespace NrvDinoPackunBattleVs1Lv1

DinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1(DinoPackun* pPackun) : DinoPackunAction("２戦目", pPackun) {
    mStateDamage = nullptr;
    initNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvStart::sInstance);
    mStateDamage = new DinoPackunStateDamage(pPackun);
    mStateDamage->setDamageNormal();
}

void DinoPackunBattleVs1Lv1::appear() {
    if (MR::isDead(getHost())) {
        getHost()->makeActorAppeared();
    }

    mIsDead = false;
    setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvStart::sInstance);
}

void DinoPackunBattleVs1Lv1::attackSensor(HitSensor* a1, HitSensor* a2) {
    if (MR::isSensorPlayer(a2)) {
        if (isNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvChase::sInstance) && sendBlowAttackMessage(a1, a2, false)) {
            setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvAttackHit::sInstance);
            return;
        }

        bool v6 = false;

        if (isNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvWalk::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvTurn::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvFind::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvCoolDown::sInstance)) {
            v6 = true;
        }

        if (v6 && sendHitAttackMessage(a1, a2, false)) {
            setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvAttackHit::sInstance);
            return;
        } else {
            MR::sendMsgPush(a2, a1);
        }
    } else {
        bool v7 = false;
        if (isNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvWalk::sInstance) ||
            isNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvChase::sInstance)) {
            v7 = true;
        }

        if (v7) {
            MR::sendMsgEnemyAttack(a2, a1);
        }
    }
}

bool DinoPackunBattleVs1Lv1::receiveMsgPlayerAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
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

bool DinoPackunBattleVs1Lv1::receiveOtherMsg(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (isNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvDamage::sInstance)) {
        return mStateDamage->receiveOtherMsg(msg, a2, a3);
    }

    if (mStateDamage->isDamageMessage(msg)) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvDamage::sInstance);
        return true;
    }

    return false;
}

bool DinoPackunBattleVs1Lv1::tryFind() {
    if (MR::isInSightConePlayer(getHost(), getHost()->_E8, 1000.0f, 80.0f)) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvFind::sInstance);
        return true;
    }

    return false;
}

void DinoPackunBattleVs1Lv1::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Find", nullptr);
    }

    if (updateStart()) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvChase::sInstance);
    }
}

void DinoPackunBattleVs1Lv1::exeTurn() {
    if (MR::isFirstStep(this)) {
        if (_10 > 0.0f) {
            MR::startBck(getHost(), "TurnRight", nullptr);
        } else {
            MR::startBck(getHost(), "TurnLeft", nullptr);
        }

        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_WALK", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    if (updateTurn(60, 1.0f)) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvWalk::sInstance);
    } else {
        if (tryFind()) {
            return;
        }
    }
}

void DinoPackunBattleVs1Lv1::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Walk", nullptr);
    }

    if (updateWalk(180, 0.5f, 91)) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvTurn::sInstance);
    } else {
        if (tryFind()) {
            return;
        }
    }
}

void DinoPackunBattleVs1Lv1::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Chase", nullptr);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_CHASE", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    bool isHit = getHost()->isHitReaction(15);
    f32 v3 = isHit ? 0.0f : 1.5f;
    f32 v4 = isHit ? 0.0f : 1.0f;

    if (updateChase(240, 180.0f, v4, v3, 60, 46)) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvCoolDown::sInstance);
    }
}

void DinoPackunBattleVs1Lv1::exeDamage() {
    if (MR::updateActorState(this, mStateDamage)) {
        kill();
    }
}

void DinoPackunBattleVs1Lv1::exeAttackHit() {
    if (updateAttackHit()) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvTurn::sInstance);
    }
}

void DinoPackunBattleVs1Lv1::exeCoolDown() {
    if (updateCoolDown(0x5A)) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvTurn::sInstance);
    }
}

void DinoPackunBattleVs1Lv1::exeFind() {
    if (updateFind(0xE, 1.0f)) {
        setNerve(&NrvDinoPackunBattleVs1Lv1::DinoPackunBattleVs1Lv1NrvChase::sInstance);
    }
}

DinoPackunBattleVs1Lv1::~DinoPackunBattleVs1Lv1() {
    return;
}
