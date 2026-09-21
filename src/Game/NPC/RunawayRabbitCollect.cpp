#include "Game/NPC/RunawayRabbitCollect.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/RunawayRabbit.hpp"
#include "Game/NPC/RunawayTico.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    static s32 sStartRunAwayBgmState = 2;
    static s32 sFoundRabbitBgmState = 3;
    static s32 sCaughtRabbitBgmState = 2;
    static s32 sEndRunAwayBgmState = 1;
    static const s32 sStartRunAwayBgmChangeFrames = 60;
    static const s32 sFoundRabbitBgmChangeFrames = 30;
    static const s32 sCaughtRabbitBgmChangeFrames = 90;
    static const s32 sEndRunAwayBgmChangeFrames = 120;
};  // namespace

namespace NrvRunawayRabbitCollect {
    NEW_NERVE(RunawayRabbitCollectNrvWait, RunawayRabbitCollect, Wait);
    NEW_NERVE(RunawayRabbitCollectNrvActive, RunawayRabbitCollect, Active);
};  // namespace NrvRunawayRabbitCollect

RunawayRabbitCollect::RunawayRabbitCollect(const char* pName)
    : LiveActor(pName), mRabbit(), mRabbitNum(), mLinkedRabbitCount(), mCaughtRabbitCount(), mCompleteRabbitCount(), mBgmState() {
}

void RunawayRabbitCollect::init(const JMapInfoIter& rIter) {
    MR::connectToSceneNpcMovement(this);
    mRabbitNum = 0;
    mTicoNum = 0;
    const char* childObjName;

    for (s32 i = 0; i < MR::getChildObjNum(rIter); i++) {
        MR::getChildObjName(&childObjName, rIter, i);
        if (MR::isEqualString(childObjName, "RunawayRabbit")) {
            mRabbitNum++;
        } else if (MR::isEqualString(childObjName, "RunawayTico")) {
            mTicoNum++;
        }
    }
    mRabbit = new RunawayRabbit*[mRabbitNum];
    mTico = new RunawayTico*[mTicoNum];

    mRabbitNum = 0;
    mTicoNum = 0;

    for (s32 i = 0; i < MR::getChildObjNum(rIter); i++) {
        MR::getChildObjName(&childObjName, rIter, i);
        if (MR::isEqualString(childObjName, "RunawayRabbit")) {
            mRabbit[mRabbitNum] = new RunawayRabbit("球面逃げウサギ", this);
            MR::initChildObj(mRabbit[mRabbitNum], rIter, i);
            mRabbitNum++;
        } else if (MR::isEqualString(childObjName, "RunawayTico")) {
            mTico[mTicoNum] = new RunawayTico("逃げチコ");
            MR::initChildObj(mTico[mTicoNum], rIter, i);
            mTicoNum++;
        }
    }

    linkMsgCtrl();
    mCompleteRabbitCount = calcCompleteRabbitCount();
    initNerve(GET_NERVE(RunawayRabbitCollect, RunawayRabbitCollectNrvWait));

    mCameraInfo = MR::createActorCameraInfo(rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    MR::tryRegisterDemoCast(this, rIter);
    MR::useStageSwitchWriteA(this, rIter);
    MR::invalidateClipping(this);
    makeActorAppeared();

    for (s32 i = 0; i < ARRAY_SIZE(mIsAppearTico); i++) {
        mIsAppearTico[i] = false;
    }
}

void RunawayRabbitCollect::initAfterPlacement() {
    MR::sendMsgToAllLiveActor(ACTMES_HEAVENSDOOR_RUNAWAY_RABBIT_WAIT, nullptr);
}

s32 RunawayRabbitCollect::calcCompleteRabbitCount() const {
    s32 count = 0;
    for (s32 i = 0; i < mRabbitNum; i++) {
        s32 groupId = mRabbit[i]->getGroupId();

        if (groupId < 0) {
            count++;
        } else {
            bool isDuplicate = false;
            for (s32 j = 0; j < i; j++) {
                if (groupId == mRabbit[j]->getGroupId()) {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate) {
                count++;
            }
        }
    }

    return count;
}

void RunawayRabbitCollect::linkMsgCtrl() {
    for (s32 i = 0; i < mRabbitNum; i++) {
        RunawayRabbit* pRabbit = mRabbit[i];
        s32 groupId = pRabbit->getGroupId();

        if (groupId >= 0) {
            for (s32 j = 0; j < mTicoNum; j++) {
                if (groupId == mTico[j]->mDemoCastID) {
                    pRabbit->setMsgCtrl(mTico[j]->mMsgCtrl);
                    break;
                }
            }
        }
    }
}

void RunawayRabbitCollect::noticeAppearRabbit(RunawayRabbit* pRabbit) {
    s32 groupId = pRabbit->getGroupId();

    if (groupId == -1) {
        return;
    }

    for (s32 i = 0; i < mRabbitNum; i++) {
        if (mRabbit[i] != pRabbit && groupId == mRabbit[i]->getGroupId()) {
            mRabbit[i]->mIsActive = false;
        }
    }
}

void RunawayRabbitCollect::noticeCaughtRabbit(RunawayRabbit* pRabbit) {
    mCaughtRabbitCount++;

    for (s32 i = 0; i < mRabbitNum; i++) {
        if (mRabbit[i] != pRabbit) {
            mRabbit[i]->setNotCaughtable();
        }
    }

    for (s32 i = 0; i < mTicoNum; i++) {
        if (mTico[i]->mDemoCastID == pRabbit->getGroupId()) {
            TVec3f jointPos;
            MR::copyJointPos(pRabbit, "Spine", &jointPos);
            mTico[i]->setPosAfterCaught(jointPos);
            break;
        }
    }

    if (mCaughtRabbitCount == mCompleteRabbitCount) {
        pRabbit->setLastMessage();
    } else {
        pRabbit->setMessage();
    }

    for (s32 i = 0; i < mRabbitNum; i++) {
        mRabbit[i]->incrementRunawayLevel();
    }
}

void RunawayRabbitCollect::control() {
}

void RunawayRabbitCollect::exeWait() {
    for (s32 i = 0; i < mTicoNum; i++) {
        if (mTico[i]->isStartRunaway()) {
            for (s32 j = 0; j < mRabbitNum; j++) {
                mRabbit[j]->activate();
            }

            MR::sendMsgToAllLiveActor(ACTMES_HEAVENSDOOR_RUNAWAY_RABBIT_START, nullptr);
            setNerve(GET_NERVE(RunawayRabbitCollect, RunawayRabbitCollectNrvActive));
            return;
        }
    }
}

void RunawayRabbitCollect::appearTico(RunawayTico* pTico, const TVec3f& rPos) {
    mIsAppearTico[pTico->mDemoCastID] = true;

    for (s32 i = 0; i < ARRAY_SIZE(mIsAppearTico); i++) {
        if (!mIsAppearTico[i]) {
            switch (i) {
            case 0:
                pTico->appearHoleComment(rPos);
                break;
            case 1:
                pTico->appearPipeComment(rPos);
                break;
            case 2:
                pTico->appearBushComment(rPos);
                break;
            }
            return;
        }
    }

    pTico->appearMamaComment(rPos);
}

void RunawayRabbitCollect::exeActive() {
    if (MR::isFirstStep(this)) {
        MR::setStageBGMState(::sStartRunAwayBgmState, ::sStartRunAwayBgmChangeFrames);
        mBgmState = ::sStartRunAwayBgmState;
    }

    bool caughtAll = true;
    bool isLinked = false;
    s32 caughtCount = 0;
    s32 chaseCount = 0;

    for (s32 i = 0; i < mRabbitNum; i++) {
        if (!mRabbit[i]->mIsActive) {
            continue;
        }

        if (mRabbit[i]->isChasing()) {
            chaseCount++;
        }

        if (mRabbit[i]->isCaught()) {
            caughtCount++;
            if (!mRabbit[i]->mIsLinked) {
                mLinkedRabbitCount++;

                for (s32 j = 0; j < mTicoNum; j++) {
                    if (mTico[j]->mDemoCastID == mRabbit[i]->getGroupId()) {
                        TVec3f jointPos;
                        MR::copyJointPos(mRabbit[i], "Spine", &jointPos);
                        appearTico(mTico[j], jointPos);
                    }
                }

                mRabbit[i]->mIsLinked = true;
                isLinked = true;
            }
        }
    }

    if (caughtCount < mCompleteRabbitCount) {
        caughtAll = false;
    }

    if (isLinked) {
        if (caughtCount == mCompleteRabbitCount) {
            MR::startSystemSE("SE_SY_RUNAWAY_RABBIT_GET_3");
        } else if (caughtCount == mCompleteRabbitCount - 1) {
            MR::startSystemSE("SE_SY_RUNAWAY_RABBIT_GET_2");
        } else {
            MR::startSystemSE("SE_SY_RUNAWAY_RABBIT_GET_1");
        }
    }

    if (chaseCount > 0) {
        if (mBgmState != ::sFoundRabbitBgmState) {
            MR::setStageBGMState(::sFoundRabbitBgmState, ::sFoundRabbitBgmChangeFrames);
        }
        mBgmState = ::sFoundRabbitBgmState;
    } else {
        if (mBgmState != ::sCaughtRabbitBgmState) {
            MR::setStageBGMState(::sCaughtRabbitBgmState, ::sCaughtRabbitBgmChangeFrames);
        }
        mBgmState = ::sCaughtRabbitBgmState;
    }

    if (caughtAll) {
        MR::isValidSwitchA(this);
        MR::setStageBGMState(::sEndRunAwayBgmState, ::sEndRunAwayBgmChangeFrames);
        kill();
    }
}
