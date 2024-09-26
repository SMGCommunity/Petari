#include "Game/Boss/BossKameckVs2.hpp"
#include "Game/Boss/BossKameckBattlePattarn.hpp"
#include "Game/Boss/BossKameckBattleDemo.hpp"
#include "Game/Boss/BossKameckStateBattle.hpp"
#include "Game/Boss/BossKameck.hpp"

namespace {
    s32 sBeamPatternLv1[] = { 2, 1, -1 };
    s32 sBeamPatternLv2[] = { 3, 1, -1 };
    s32 sBeamPatternLv3[] = { 4, 1, 3, -1 };

    BossKameckBattlePattarn sPatternLv1 = BossKameckBattlePattarn(sBeamPatternLv1, false);
    BossKameckBattlePattarn sPatternLv2 = BossKameckBattlePattarn(sBeamPatternLv2, false);
    BossKameckBattlePattarn sPatternLv3 = BossKameckBattlePattarn(sBeamPatternLv3, true);
};

BossKameckVs2::BossKameckVs2() : BossKameckSequencer("ボスカメックVs2") {
    mStateBattle = nullptr;
    mBarrier = nullptr;
}

void BossKameckVs2::init(BossKameck *pBoss, const JMapInfoIter &rIter) {
    BossKameckSequencer::init(pBoss, rIter);
    mStateBattle = new BossKameckStateBattle(pBoss);
    mStateBattle->init();
    mStateBattle->_3D = true;
    mBarrier = new BossKameckBarrier("カメックバリア", "BossKameckBarrier");
    mBarrier->init(rIter);
    initNerve(&NrvBossKameckVs2::BossKameckVs2NrvOpeningDemo::sInstance);
    MR::declareStarPiece(mBossKameck, 24);
}

void BossKameckVs2::start() {
    setNerve(&NrvBossKameckVs2::BossKameckVs2NrvOpeningDemo::sInstance);
}

void BossKameckVs2::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (isBattle()) {
        mStateBattle->attackSensor(a1, a2);
    }
}

bool BossKameckVs2::receiveMsgPlayerAttack(u32 msg, HitSensor *a1, HitSensor *a2) {
    if (isBattle()) {
        return mStateBattle->receiveMsgPlayerAttack(msg, a1, a2);
    }

    return false;
}

void BossKameckVs2::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        mBattleDemo->startDemoAppearVs2();
    }

    if (MR::updateActorState(this, mBattleDemo)) {
        setNerve(&NrvBossKameckVs2::BossKameckVs2NrvBattleLv1::sInstance);
        mBarrier->appear();
    }
}

void BossKameckVs2::exeBattleLv1() {
    if (MR::isFirstStep(this)) {
        mStateBattle->setMoveRail(mBossKameck->getMoveRail(0));
        mStateBattle->setBattlePattarn(&sPatternLv1);
        mStateBattle->_3C = 0;
        mStateBattle->_30 = 3;
    }

    MR::updateActorStateAndNextNerve(this, mStateBattle, &NrvBossKameckVs2::BossKameckVs2NrvBattleLv2::sInstance);
}

void BossKameckVs2::exeBattleLv2() {
    if (MR::isFirstStep(this)) {
        mBossKameck->appearStarPieceToPlayer(8);
        mStateBattle->setMoveRail(mBossKameck->getMoveRail(1));
        mStateBattle->setBattlePattarn(&sPatternLv2);
        mStateBattle->_3C = 0;
        mStateBattle->_30 = 3;
    }

    MR::updateActorStateAndNextNerve(this, mStateBattle, &NrvBossKameckVs2::BossKameckVs2NrvPowerUpDemo::sInstance);
}

void BossKameckVs2::exePowerUpDemo() {
    if (MR::isFirstStep(this)) {
        mBattleDemo->startDemoPowerUpVs2();
    }

    if (MR::updateActorState(this, mBattleDemo)) {
        setNerve(&NrvBossKameckVs2::BossKameckVs2NrvBattleLv3::sInstance);
    }
}

void BossKameckVs2::exeBattleLv3() {
    if (MR::isFirstStep(this)) {
        mStateBattle->setMoveRail(mBossKameck->getMoveRail(2));
        mStateBattle->setBattlePattarn(&sPatternLv3);
        mStateBattle->_3C = 0;
        mStateBattle->_30 = 3;
    }

    MR::updateActorStateAndNextNerve(this, mStateBattle, &NrvBossKameckVs2::BossKameckVs2NrvBattleLv4::sInstance);
}

void BossKameckVs2::exeBattleLv4() {
    if (MR::isFirstStep(this)) {
        mBossKameck->appearStarPieceToPlayer(16);
        mStateBattle->setMoveRail(mBossKameck->getMoveRail(3));
        mStateBattle->setBattlePattarn(&sPatternLv3);
        mStateBattle->_3C = 1;
        mStateBattle->_30 = 4;
    }

    MR::updateActorStateAndNextNerve(this, mStateBattle, &NrvBossKameckVs2::BossKameckVs2NrvEndDemo::sInstance);
}

void BossKameckVs2::exeEndDemo() {
    if (MR::isFirstStep(this)) {
        mBattleDemo->startDemoDownVs2();
        mBarrier->kill();
    }

    MR::updateActorState(this, mBattleDemo);
}

bool BossKameckVs2::isBattle() const {
    if (isNerve(&NrvBossKameckVs2::BossKameckVs2NrvBattleLv1::sInstance)
        || isNerve(&NrvBossKameckVs2::BossKameckVs2NrvBattleLv2::sInstance)
        || isNerve(&NrvBossKameckVs2::BossKameckVs2NrvBattleLv3::sInstance)
        || isNerve(&NrvBossKameckVs2::BossKameckVs2NrvBattleLv4::sInstance)) {
            return true;
        }
    return false;
}

BossKameckVs2::~BossKameckVs2() {

}

namespace NrvBossKameckVs2 {
    INIT_NERVE(BossKameckVs2NrvOpeningDemo);
    INIT_NERVE(BossKameckVs2NrvBattleLv1);
    INIT_NERVE(BossKameckVs2NrvBattleLv2);
    INIT_NERVE(BossKameckVs2NrvPowerUpDemo);
    INIT_NERVE(BossKameckVs2NrvBattleLv3);
    INIT_NERVE(BossKameckVs2NrvBattleLv4);
    INIT_NERVE(BossKameckVs2NrvEndDemo);
};