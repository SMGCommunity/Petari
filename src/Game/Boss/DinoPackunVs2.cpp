#include "Game/Boss/DinoPackunVs2.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunBall.hpp"
#include "Game/LiveActor/PartsModel.hpp"

namespace NrvDinoPackunVs2 {
    NEW_NERVE(DinoPackunVs2NrvWaitStart, DinoPackunVs2, WaitStart);
    NEW_NERVE(DinoPackunVs2NrvOpeningDemo, DinoPackunVs2, OpeningDemo);
    NEW_NERVE(DinoPackunVs2NrvBattleEgg, DinoPackunVs2, BattleEgg);
    NEW_NERVE(DinoPackunVs2NrvCryDemo, DinoPackunVs2, CryDemo);
    NEW_NERVE(DinoPackunVs2NrvBattleLv1, DinoPackunVs2, BattleLv1);
    NEW_NERVE(DinoPackunVs2NrvAngryDemo, DinoPackunVs2, AngryDemo);
    NEW_NERVE(DinoPackunVs2NrvBattleLv2, DinoPackunVs2, BattleLv2);
    NEW_NERVE(DinoPackunVs2NrvBattleLv3, DinoPackunVs2, BattleLv3);
    NEW_NERVE(DinoPackunVs2NrvDownDemo, DinoPackunVs2, DownDemo);
};  // namespace NrvDinoPackunVs2

DinoPackunVs2::DinoPackunVs2(DinoPackun* pPackun) : DinoPackunSequencer("ディノパックン2戦目進行", pPackun) {
    mDemo = nullptr;
    mEgg = nullptr;
    mBattleLv1 = nullptr;
    mTrackFireHolder = nullptr;
}

void DinoPackunVs2::init() {
    initNerve(&NrvDinoPackunVs2::DinoPackunVs2NrvWaitStart::sInstance);
    mTrackFireHolder = new DinoPackunTrackFireHolder(64);
    mFireHolder = new DinoPackunFireHolder(16);
    mDemo = new DinoPackunDemo(mParent);
    mDemo->init();
    mEgg = new DinoPackunBattleEggVs2(mParent);
    mEgg->setTrackFire(mTrackFireHolder);
    mEgg->init();
    mBattleLv1 = new DinoPackunBattleVs2Lv1(mParent);
    mBattleLv1->setTrackFire(mTrackFireHolder);
    mBattleLv1->setFireBall(mFireHolder);
    mBattleLv1->init();
    mParent->mBall->_124 = 1;
    MR::declareStarPiece(mParent, 24);
}

void DinoPackunVs2::start() {
    if (isNerve(&NrvDinoPackunVs2::DinoPackunVs2NrvWaitStart::sInstance)) {
        setNerve(&NrvDinoPackunVs2::DinoPackunVs2NrvOpeningDemo::sInstance);
    }
}

void DinoPackunVs2::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startOpeningDemo();
        MR::startBrk(mParent, "Awake");
        MR::emitEffect(mParent->getBallModel(), "TailFire");
        MR::startBrk(mParent->getBallModel(), "Heat");
    }

    MR::updateActorStateAndNextNerve(this, mDemo, &NrvDinoPackunVs2::DinoPackunVs2NrvBattleEgg::sInstance);
}

void DinoPackunVs2::exeBattleEgg() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mEgg;
    }

    MR::updateActorStateAndNextNerve(this, mEgg, &NrvDinoPackunVs2::DinoPackunVs2NrvCryDemo::sInstance);
}

void DinoPackunVs2::exeCryDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startCryDemo();
    }

    MR::updateActorStateAndNextNerve(this, mDemo, &NrvDinoPackunVs2::DinoPackunVs2NrvBattleLv1::sInstance);
}

void DinoPackunVs2::exeBattleLv1() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mBattleLv1->_38 = 0;
        mBattleLv1->_39 = 0;
        mBattleLv1->_3D = 0;
        mBattleLv1->_3B = 0;
        mBattleLv1->_3A = 0;
        mBattleLv1->setMiddleBattle();
        mBattleLv1->_3C = 0;
        mCurrentAction = mBattleLv1;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv1, &NrvDinoPackunVs2::DinoPackunVs2NrvAngryDemo::sInstance);
}

void DinoPackunVs2::exeAngryDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startAngryDemo();
        MR::emitEffect(mParent->getBallModel(), "TailFire");
        MR::startBrk(mParent->getBallModel(), "Heat");
    }

    MR::updateActorStateAndNextNerve(this, mDemo, &NrvDinoPackunVs2::DinoPackunVs2NrvBattleLv2::sInstance);
}

void DinoPackunVs2::exeBattleLv2() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mBattleLv1->_38 = 1;
        mBattleLv1->_39 = 0;
        mBattleLv1->_3D = 0;
        mBattleLv1->_3B = 1;
        mBattleLv1->_3A = 1;
        mBattleLv1->setMiddleBattle();
        mBattleLv1->_3C = 1;
        mCurrentAction = mBattleLv1;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv1, &NrvDinoPackunVs2::DinoPackunVs2NrvBattleLv3::sInstance);
}

void DinoPackunVs2::exeBattleLv3() {
    if (MR::isFirstStep(this)) {
        mParent->appearStarPiece(8);
        mBattleLv1->_38 = 1;
        mBattleLv1->_39 = 1;
        mBattleLv1->_3D = 1;
        mBattleLv1->_3B = 1;
        mBattleLv1->_3A = 1;
        mBattleLv1->setLastBattle();
        mBattleLv1->_3C = 0;
        mCurrentAction = mBattleLv1;
    }

    MR::updateActorStateAndNextNerve(this, mBattleLv1, &NrvDinoPackunVs2::DinoPackunVs2NrvDownDemo::sInstance);
}

void DinoPackunVs2::exeDownDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = nullptr;
        mDemo->startDownDemo();
    }

    if (MR::updateActorState(this, mDemo)) {
        return;
    }
}

void DinoPackunVs2::exeWaitStart() {
    return;
}

DinoPackunVs2::~DinoPackunVs2() {
    return;
}

s32 DinoPackunVs2::getVsCount() const {
    return 2;
}

bool DinoPackunVs2::isUseEggShell() const {
    return true;
}
