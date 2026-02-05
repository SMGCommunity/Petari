#include "Game/Boss/BossStinkBugActionSequencer.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionFlyHigh.hpp"
#include "Game/Boss/BossStinkBugActionFlyLow.hpp"
#include "Game/Boss/BossStinkBugActionGround.hpp"
#include "Game/Boss/BossStinkBugAngryDemo.hpp"
#include "Game/Boss/BossStinkBugFinishDemo.hpp"
#include "Game/Boss/BossStinkBugFlyDemo.hpp"
#include "Game/Boss/BossStinkBugOpeningDemo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

namespace NrvBossStinkBugActionSequencer {
    NEW_NERVE(BossStinkBugActionSequencerNrvWaitStart, BossStinkBugActionSequencer, WaitStart);
    NEW_NERVE(BossStinkBugActionSequencerNrvOpeningDemo, BossStinkBugActionSequencer, OpeningDemo);
    NEW_NERVE(BossStinkBugActionSequencerNrvGround, BossStinkBugActionSequencer, Ground);
    NEW_NERVE(BossStinkBugActionSequencerNrvFlyDemo, BossStinkBugActionSequencer, FlyDemo);
    NEW_NERVE(BossStinkBugActionSequencerNrvFlyLow, BossStinkBugActionSequencer, FlyLow);
    NEW_NERVE(BossStinkBugActionSequencerNrvAngryDemo, BossStinkBugActionSequencer, AngryDemo);
    NEW_NERVE(BossStinkBugActionSequencerNrvFlyHigh, BossStinkBugActionSequencer, FlyHigh);
    NEW_NERVE(BossStinkBugActionSequencerNrvFinishDemo, BossStinkBugActionSequencer, FinishDemo);
    NEW_NERVE(BossStinkBugActionSequencerNrvEnd, BossStinkBugActionSequencer, End);
}  // namespace NrvBossStinkBugActionSequencer

BossStinkBugActionSequencer::BossStinkBugActionSequencer(BossStinkBug* pStinkBug, const JMapInfoIter& rIter)
    : NerveExecutor("ボスカメムシアクション管理"), mStinkBug(pStinkBug), mCurrentAction(nullptr), mOpeningDemo(nullptr), mActionGround(nullptr),
      mFlyDemo(nullptr), mActionFlyLow(nullptr), mAngryDemo(nullptr), mActionFlyHigh(nullptr), mFinishDemo(nullptr) {
    mOpeningDemo = new BossStinkBugOpeningDemo(pStinkBug, rIter);
    mActionGround = new BossStinkBugActionGround(pStinkBug);
    mFlyDemo = new BossStinkBugFlyDemo(pStinkBug, rIter);
    mActionFlyLow = new BossStinkBugActionFlyLow(pStinkBug);
    mAngryDemo = new BossStinkBugAngryDemo(pStinkBug, rIter);
    mActionFlyHigh = new BossStinkBugActionFlyHigh(pStinkBug);
    mFinishDemo = new BossStinkBugFinishDemo(pStinkBug, rIter);
    initNerve(&NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvWaitStart::sInstance);
    MR::declareStarPiece(mStinkBug, 24);
}

bool BossStinkBugActionSequencer::isValidFollowId(s32 id) const {
    if (id == -1) {
        return true;
    }

    if (mCurrentAction != nullptr) {
        return mCurrentAction->isValidFollowId(id);
    }

    return false;
}

void BossStinkBugActionSequencer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        mCurrentAction->attackSensor(pSender, pReceiver);
    }
}

bool BossStinkBugActionSequencer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    } else {
        return false;
    }
}

bool BossStinkBugActionSequencer::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveOtherMsg(msg, pSender, pReceiver);
    } else {
        return false;
    }
}

bool BossStinkBugActionSequencer::startAction() {
    if (isNerve(&NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvWaitStart::sInstance)) {
        setNerve(&NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvOpeningDemo::sInstance);
        return true;
    } else {
        return false;
    }
}

void BossStinkBugActionSequencer::exeWaitStart() {
}

void BossStinkBugActionSequencer::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mOpeningDemo;
    }
    MR::updateActorStateAndNextNerve(this, mOpeningDemo, &NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvGround::sInstance);
}

void BossStinkBugActionSequencer::exeGround() {
    if (MR::isFirstStep(this)) {
        MR::startBossBGM(0);
        mCurrentAction = mActionGround;
    }
    MR::updateActorStateAndNextNerve(this, mActionGround, &NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvFlyDemo::sInstance);
}

void BossStinkBugActionSequencer::exeFlyDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mFlyDemo;
    }
    MR::updateActorStateAndNextNerve(this, mFlyDemo, &NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvFlyLow::sInstance);
}

void BossStinkBugActionSequencer::exeFlyLow() {
    if (MR::isFirstStep(this)) {
        mStinkBug->appearStarPiece(8);
        MR::startBossBGM(1);
        mCurrentAction = mActionFlyLow;
    }
    MR::updateActorStateAndNextNerve(this, mActionFlyLow, &NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvAngryDemo::sInstance);
}

void BossStinkBugActionSequencer::exeAngryDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mAngryDemo;
    }
    MR::updateActorStateAndNextNerve(this, mAngryDemo, &NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvFlyHigh::sInstance);
}

void BossStinkBugActionSequencer::exeFlyHigh() {
    if (MR::isFirstStep(this)) {
        mStinkBug->appearStarPiece(16);
        mCurrentAction = mActionFlyHigh;
    }
    MR::updateActorStateAndNextNerve(this, mActionFlyHigh, &NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvFinishDemo::sInstance);
}

void BossStinkBugActionSequencer::exeFinishDemo() {
    if (MR::isFirstStep(this)) {
        mCurrentAction = mFinishDemo;
    }
    MR::updateActorStateAndNextNerve(this, mFinishDemo, &NrvBossStinkBugActionSequencer::BossStinkBugActionSequencerNrvEnd::sInstance);
}

void BossStinkBugActionSequencer::exeEnd() {
}
