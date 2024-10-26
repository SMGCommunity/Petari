#include "Game/Map/TimerSwitch.hpp"

TimerSwitch::TimerSwitch(const char *pName) : LiveActor(pName) {
    mTimerLength = -1;
    mCurrentTime = -1;
}

void TimerSwitch::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &mTimerLength);
    MR::needStageSwitchWriteA(this, rIter);
    MR::needStageSwitchReadB(this, rIter);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void TimerSwitch::control() {
    if (mCurrentTime < 0 && MR::isOnSwitchB(this)) {
        mCurrentTime = mTimerLength;
    }

    s32 current = mCurrentTime;
    if (current > 0) {
        mCurrentTime = current - 1;

        if (current - 1 <= 0) {
            MR::onSwitchA(this);
            kill();
        }
    }
}

TimerSwitch::~TimerSwitch() {

}
