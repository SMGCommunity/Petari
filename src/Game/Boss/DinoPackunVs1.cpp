#include "Game/Boss/DinoPackunVs1.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunBattleVs1Lv1.hpp"
#include "Game/Boss/DinoPackunBattleVs1Lv2.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

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
    initNerve(GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvWaitStart));
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
    if (isNerve(GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvWaitStart))) {
        setNerve(GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvOpeningDemo));
    }
}

void DinoPackunVs1::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startOpeningDemo();
    }

    MR::updateActorStateAndNextNerve(this, mDemo, GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvBattleEgg));
}

void DinoPackunVs1::exeBattleEgg() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mEgg;
    }

    MR::updateActorStateAndNextNerve(this, mEgg, GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvCryDemo));
}

void DinoPackunVs1::exeCryDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startCryDemo();
    }

    MR::updateActorStateAndNextNerve(this, mDemo, GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvBattleLv1));
}

void DinoPackunVs1::exeBattleLv1() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mCurrentAction = mBattleLv1;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv1, GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvAngryDemo));
}

void DinoPackunVs1::exeAngryDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startAngryDemo();
    }

    MR::updateActorStateAndNextNerve(this, mDemo, GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvBattleLv2));
}

void DinoPackunVs1::exeBattleLv2() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mBattleLv2->setMiddleBattle();
        mBattleLv2->setStartFromDemo();
        mCurrentAction = mBattleLv2;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv2, GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvBattleLv3));
}

void DinoPackunVs1::exeBattleLv3() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mBattleLv2->setLastBattle();
        mBattleLv2->setStartContinueBattle();
        mCurrentAction = mBattleLv2;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv2, GET_NERVE(DinoPackunVs1, DinoPackunVs1NrvDownDemo));
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
