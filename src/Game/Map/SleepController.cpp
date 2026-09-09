#include "Game/Map/SleepController.hpp"
#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Map/StageSwitch.hpp"

SleepController::SleepController(const JMapInfoIter& rIter, SwitchEventListener* pListener)
    : mSwitchEventListener(pListener), mSwitchIdInfo(), mIsOnSwitchByIdInfo() {
    mSwitchIdInfo = StageSwitchFunction::createSwitchIdInfo("SW_SLEEP", rIter, false);
}

void SleepController::initSync() {
    if (StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSwitchIdInfo)) {
        mSwitchEventListener->listenSwitchOnEvent();
    } else {
        mSwitchEventListener->listenSwitchOffEvent();
    }
}

void SleepController::update() {
    bool isOnSwitchByIdInfo = StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSwitchIdInfo);

    if (!mIsOnSwitchByIdInfo && isOnSwitchByIdInfo) {
        mSwitchEventListener->listenSwitchOnEvent();
    }

    if (mIsOnSwitchByIdInfo && !isOnSwitchByIdInfo) {
        mSwitchEventListener->listenSwitchOffEvent();
    }

    mIsOnSwitchByIdInfo = isOnSwitchByIdInfo;
}
