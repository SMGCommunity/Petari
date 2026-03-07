#include "Game/Boss/PoltaActionSequencer.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/PoltaBattleLv1.hpp"
#include "Game/Boss/PoltaBattleLv2.hpp"
#include "Game/Boss/PoltaDemo.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/Boss/PoltaWaitStart.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "revolution/types.h"

namespace NrvPoltaActionSequencer {
    NEW_NERVE(PoltaActionSequencerNrvWaitStart, PoltaActionSequencer, WaitStart);
    NEW_NERVE(PoltaActionSequencerNrvOpeningDemo, PoltaActionSequencer, OpeningDemo);
    NEW_NERVE(PoltaActionSequencerNrvBattleLv1, PoltaActionSequencer, BattleLv1);
    NEW_NERVE(PoltaActionSequencerNrvPowerUpDemo, PoltaActionSequencer, PowerUpDemo);
    NEW_NERVE(PoltaActionSequencerNrvBattleLv2, PoltaActionSequencer, BattleLv2);
    NEW_NERVE(PoltaActionSequencerNrvDownDemo, PoltaActionSequencer, DownDemo);
}  // namespace NrvPoltaActionSequencer

PoltaActionSequencer::PoltaActionSequencer(Polta* pPolta, const JMapInfoIter& rIter)
    : NerveExecutor("ポルタアクション管理"), mPoltaPtr(pPolta), mActionDemo(nullptr), mCurrentAction(nullptr), mActionWaitStart(nullptr),
      mActionPoltaBattleLv1(nullptr), mActionPoltaBattleLv2(nullptr) {
    initNerve(&NrvPoltaActionSequencer::PoltaActionSequencerNrvWaitStart::sInstance);
    mActionDemo = new PoltaDemo(pPolta);
    mActionDemo->init();
    mActionWaitStart = new PoltaWaitStart(pPolta);
    mActionPoltaBattleLv1 = new PoltaBattleLv1(pPolta);
    mActionPoltaBattleLv2 = new PoltaBattleLv2(pPolta);
    MR::declareStarPiece(mPoltaPtr, 24);
}

void PoltaActionSequencer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        mCurrentAction->attackSensor(pSender, pReceiver);
    }
}

bool PoltaActionSequencer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }
    return false;
}

bool PoltaActionSequencer::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveMsgEnemyAttack(msg, pSender, pReceiver);
    }
    return false;
}

bool PoltaActionSequencer::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveOtherMsg(msg, pSender, pReceiver);
    }
    return false;
}

bool PoltaActionSequencer::updateAction() {
    return mCurrentAction->update();
}

bool PoltaActionSequencer::startAction() {
    if (isNerve(&NrvPoltaActionSequencer::PoltaActionSequencerNrvWaitStart::sInstance)) {
        setNerve(&NrvPoltaActionSequencer::PoltaActionSequencerNrvOpeningDemo::sInstance);
        return true;
    }
    return false;
}

void PoltaActionSequencer::exeWaitStart() {
    if (MR::isFirstStep(this)) {
        mActionWaitStart->appear();
        mCurrentAction = mActionWaitStart;
    }
}

void PoltaActionSequencer::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mActionDemo;
        mActionDemo->startOpeningDemo();
    }
    MR::updateActorStateAndNextNerve(this, mActionDemo, &NrvPoltaActionSequencer::PoltaActionSequencerNrvBattleLv1::sInstance);
}

void PoltaActionSequencer::exeBattleLv1() {
    if (MR::isFirstStep(this)) {
        mActionPoltaBattleLv1->appear();
        mCurrentAction = mActionPoltaBattleLv1;
    }
    if (updateAction()) {
        setNerve(&NrvPoltaActionSequencer::PoltaActionSequencerNrvPowerUpDemo::sInstance);
    }
}

void PoltaActionSequencer::exePowerUpDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mActionDemo;
        mActionDemo->startPowerUpDemo();
    }
    MR::updateActorStateAndNextNerve(this, mActionDemo, &NrvPoltaActionSequencer::PoltaActionSequencerNrvBattleLv2::sInstance);
}

void PoltaActionSequencer::exeBattleLv2() {
    if (MR::isFirstStep(this)) {
        mActionPoltaBattleLv2->appear();
        mCurrentAction = mActionPoltaBattleLv2;
        mPoltaPtr->appearStarPiece(10);
    }
    if (updateAction()) {
        setNerve(&NrvPoltaActionSequencer::PoltaActionSequencerNrvDownDemo::sInstance);
    }
}

void PoltaActionSequencer::exeDownDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mActionDemo;
        mActionDemo->startDownDemo();
    }
    MR::updateActorState(this, mActionDemo);
}
