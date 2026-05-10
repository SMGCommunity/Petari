#include "Game/Boss/DinoPackunVs1.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunBattleVs1Lv1.hpp"
#include "Game/Boss/DinoPackunBattleVs1Lv2.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvDinoPackunVs1 {
    NEW_NERVE(DinoPackunVs1NrvWaitStart, DinoPackunVs1, WaitStart);
    NEW_NERVE(DinoPackunVs1NrvOpeningDemo, DinoPackunVs1, OpeningDemo);
    NEW_NERVE(DinoPackunVs1NrvBattleEgg, DinoPackunVs1, BattleEgg);
    NEW_NERVE(DinoPackunVs1NrvCryDemo, DinoPackunVs1, CryDemo);
    NEW_NERVE(DinoPackunVs1NrvBattleLv1, DinoPackunVs1, BattleLv1);
    NEW_NERVE(DinoPackunVs1NrvAngryDemo, DinoPackunVs1, AngryDemo);
    NEW_NERVE(DinoPackunVs1NrvBattleLv2, DinoPackunVs1, BattleLv2);
    NEW_NERVE(DinoPackunVs1NrvBattleLv3, DinoPackunVs1, BattleLv3);
    NEW_NERVE(DinoPackunVs1NrvDownDemo, DinoPackunVs1, DownDemo);
};  // namespace NrvDinoPackunVs1

DinoPackunVs1::DinoPackunVs1(DinoPackun* pPackun) : DinoPackunSequencer("ディノパックン1戦目進行", pPackun) {
    mDemo = nullptr;
    mEgg = nullptr;
    mBattleLv1 = nullptr;
    mBattleLv2 = nullptr;
}

void DinoPackunVs1::init() {
    initNerve(&NrvDinoPackunVs1::DinoPackunVs1NrvWaitStart::sInstance);
    mDemo = new DinoPackunDemo(mParent);
    mDemo->init();
    mEgg = new DinoPackunBattleEgg(mParent);
    mEgg->init();
    mBattleLv1 = new DinoPackunBattleVs1Lv1(mParent);
    mBattleLv1->init();
    mBattleLv2 = new DinoPackunBattleVs1Lv2(mParent);
    mBattleLv2->init();
    MR::declareStarPiece(mParent, 24);
}

void DinoPackunVs1::start() {
    if (isNerve(&NrvDinoPackunVs1::DinoPackunVs1NrvWaitStart::sInstance)) {
        setNerve(&NrvDinoPackunVs1::DinoPackunVs1NrvOpeningDemo::sInstance);
    }
}

void DinoPackunVs1::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startOpeningDemo();
    }

    MR::updateActorStateAndNextNerve(this, mDemo, &NrvDinoPackunVs1::DinoPackunVs1NrvBattleEgg::sInstance);
}

void DinoPackunVs1::exeBattleEgg() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mEgg;
    }

    MR::updateActorStateAndNextNerve(this, mEgg, &NrvDinoPackunVs1::DinoPackunVs1NrvCryDemo::sInstance);
}

void DinoPackunVs1::exeCryDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startCryDemo();
    }

    MR::updateActorStateAndNextNerve(this, mDemo, &NrvDinoPackunVs1::DinoPackunVs1NrvBattleLv1::sInstance);
}

void DinoPackunVs1::exeBattleLv1() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mCurrentAction = mBattleLv1;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv1, &NrvDinoPackunVs1::DinoPackunVs1NrvAngryDemo::sInstance);
}

void DinoPackunVs1::exeAngryDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startAngryDemo();
    }

    MR::updateActorStateAndNextNerve(this, mDemo, &NrvDinoPackunVs1::DinoPackunVs1NrvBattleLv2::sInstance);
}

void DinoPackunVs1::exeBattleLv2() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mBattleLv2->setMiddleBattle();
        mBattleLv2->setStartFromDemo();
        mCurrentAction = mBattleLv2;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv2, &NrvDinoPackunVs1::DinoPackunVs1NrvBattleLv3::sInstance);
}

void DinoPackunVs1::exeBattleLv3() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mBattleLv2->setLastBattle();
        mBattleLv2->setStartContinueBattle();
        mCurrentAction = mBattleLv2;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv2, &NrvDinoPackunVs1::DinoPackunVs1NrvDownDemo::sInstance);
}

void DinoPackunVs1::exeDownDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startDownDemo();
    }

    if (MR::updateActorState(this, mDemo)) {
        return;
    }
}

void DinoPackunVs1::exeWaitStart() {
    return;
}

DinoPackunVs1::~DinoPackunVs1() {
    return;
}

s32 DinoPackunVs1::getVsCount() const {
    return 1;
}

bool DinoPackunVs1::isUseEggShell() const {
    return true;
}
