#include "Game/Boss/BossKameckVs1.hpp"
#include "Game/Boss/BossKameckBattleDemo.hpp"
#include "Game/Boss/BossKameckBattlePattarn.hpp"
#include "Game/Boss/BossKameckStateBattle.hpp"
#include "Game/Boss/BossKameck.hpp"

namespace {
    s32 sBeamPatternLv1[] = { 2, 1, -1 };
    s32 sBeamPatternLv2[] = { 3, 1, -1 };
    s32 sBeamPatternLv3[] = { 4, 1, 3, -1 };

    BossKameckBattlePattarn sPatternLv1 = BossKameckBattlePattarn(sBeamPatternLv1, false);
    BossKameckBattlePattarn sPatternLv2 = BossKameckBattlePattarn(sBeamPatternLv2, false);
    BossKameckBattlePattarn sPatternLv3 = BossKameckBattlePattarn(sBeamPatternLv3, false);
};

BossKameckVs1::BossKameckVs1() : BossKameckSequencer("ボスカメックVs1") {
    mStateBattle = nullptr;
}

void BossKameckVs1::init(BossKameck *pBoss, const JMapInfoIter &rIter) {
    BossKameckSequencer::init(pBoss, rIter);
    mStateBattle = new BossKameckStateBattle(pBoss);
    mStateBattle->init();
    initNerve(&NrvBossKameckVs1::BossKameckVs1NrvOpeningDemo::sInstance);
    MR::declareStarPiece(mBossKameck, 24);
}

void BossKameckVs1::start() {
    setNerve(&NrvBossKameckVs1::BossKameckVs1NrvOpeningDemo::sInstance);
}

void BossKameckVs1::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (isBattle()) {
        mStateBattle->attackSensor(a1, a2);
    }
}

bool BossKameckVs1::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (isBattle()) {
        return mStateBattle->receiveMsgPlayerAttack(msg, a2, a3);
    }

    return false;
}

void BossKameckVs1::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        mBattleDemo->startDemoAppearVs1();
    }

    if (MR::updateActorState(this, mBattleDemo)) {
        setNerve(&NrvBossKameckVs1::BossKameckVs1NrvBattleLv1::sInstance);
    }
}

void BossKameckVs1::exeBattleLv1() {
    if (MR::isFirstStep(this)) {
        mStateBattle->setMoveRail(mBossKameck->getMoveRail(0));
        mStateBattle->setBattlePattarn(&sPatternLv1);
        mStateBattle->_3C = 0;
    }

    MR::updateActorStateAndNextNerve(this, mStateBattle, &NrvBossKameckVs1::BossKameckVs1NrvBattleLv2::sInstance);
}

void BossKameckVs1::exeBattleLv2() {
    if (MR::isFirstStep(this)) {
        mBossKameck->appearStarPieceToPlayer(8);
        mStateBattle->setMoveRail(mBossKameck->getMoveRail(1));
        mStateBattle->setBattlePattarn(&sPatternLv2);
        mStateBattle->_3C = 0;
    }

    MR::updateActorStateAndNextNerve(this, mStateBattle, &NrvBossKameckVs1::BossKameckVs1NrvPowerUpDemo::sInstance);
}

void BossKameckVs1::exePowerUpDemo() {
    if (MR::isFirstStep(this)) {
        mBattleDemo->startDemoPowerUpVs1();
    }

    if (MR::updateActorState(this, mBattleDemo)) {
        setNerve(&NrvBossKameckVs1::BossKameckVs1NrvBattleLv3::sInstance);
    }
}

void BossKameckVs1::exeBattleLv3() {
    if (MR::isFirstStep(this)) {
        mBossKameck->appearStarPieceToPlayer(16);
        mStateBattle->setMoveRail(mBossKameck->getMoveRail(2));
        mStateBattle->setBattlePattarn(&sPatternLv3);
        mStateBattle->_3C = 1;
    }

    MR::updateActorStateAndNextNerve(this, mStateBattle, &NrvBossKameckVs1::BossKameckVs1NrvEndDemo::sInstance);
}

void BossKameckVs1::exeEndDemo() {
    if (MR::isFirstStep(this)) {
        mBattleDemo->startDemoDownVs1();
    }

    MR::updateActorState(this, mBattleDemo);
}

bool BossKameckVs1::isBattle() const {
    if (isNerve(&NrvBossKameckVs1::BossKameckVs1NrvBattleLv1::sInstance) 
    || isNerve(&NrvBossKameckVs1::BossKameckVs1NrvBattleLv2::sInstance)
    || isNerve(&NrvBossKameckVs1::BossKameckVs1NrvBattleLv3::sInstance)) {
        return true;
    }

    return false;
}

BossKameckVs1::~BossKameckVs1() {
    
} 

namespace NrvBossKameckVs1 {
    INIT_NERVE(BossKameckVs1NrvOpeningDemo);
    INIT_NERVE(BossKameckVs1NrvBattleLv1);
    INIT_NERVE(BossKameckVs1NrvBattleLv2);
    INIT_NERVE(BossKameckVs1NrvPowerUpDemo);
    INIT_NERVE(BossKameckVs1NrvBattleLv3);
    INIT_NERVE(BossKameckVs1NrvEndDemo);
};