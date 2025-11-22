#include "Game/NPC/CometEventExecutorTimeLimit.hpp"
#include "Game/Screen/TimeLimitLayout.hpp"

namespace NrvCometEventExecutorTimeLimit {
    NEW_NERVE(CometEventExecutorTimeLimitStartEvent, CometEventExecutorTimeLimit, StartEvent);
};

CometEventExecutorTimeLimit::CometEventExecutorTimeLimit(u32 timeLimit)
    : LiveActor("制限時間コメット実行者"), mTimeLimitLayout(nullptr), mTimeLimit(timeLimit) {}

void CometEventExecutorTimeLimit::init(const JMapInfoIter&) {
    MR::connectToSceneLayoutMovement(this);
    MR::invalidateClipping(this);

    mTimeLimitLayout = new TimeLimitLayout(getTimeLimit());
    mTimeLimitLayout->setDisplayModeOnNormal(true);
    mTimeLimitLayout->initWithoutIter();
    mTimeLimitLayout->kill();

    MR::connectToSceneLayout(mTimeLimitLayout);
    initNerve(&NrvCometEventExecutorTimeLimit::CometEventExecutorTimeLimitStartEvent::sInstance);
    kill();
}

void CometEventExecutorTimeLimit::appear() {
    LiveActor::appear();
    setNerve(&NrvCometEventExecutorTimeLimit::CometEventExecutorTimeLimitStartEvent::sInstance);
}

void CometEventExecutorTimeLimit::kill() {
    LiveActor::kill();
    mTimeLimitLayout->kill();
}

void CometEventExecutorTimeLimit::exeStartEvent() {
    if (MR::isFirstStep(this)) {
        mTimeLimitLayout->appear();
        mTimeLimitLayout->setTimeLimit(getTimeLimit());
    }

    if (MR::canStartDemo() && mTimeLimitLayout->isReadyToTimeUp()) {
        MR::forceKillPlayerByWaterRace();
    }
}

u32 CometEventExecutorTimeLimit::getTimeLimit() const {
    if (mTimeLimit != 0) {
        return mTimeLimit * 60;
    }

    return 108000;
}
