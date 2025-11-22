#include "Game/Map/SleepController.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "revolution/types.h"

SleepController::SleepController(const JMapInfoIter& rIter, SwitchEventListener* param2)
    : _4(param2),
      _0(nullptr),
      _8(false) {
    _0 = StageSwitchFunction::createSwitchIdInfo("SW_SLEEP", rIter, false);
}

void SleepController::initSync() {

    if (StageSwitchFunction::isOnSwitchBySwitchIdInfo(*_0)) {
        _4->listenSwitchOnEvent();
        return;
    }
    _4->listenSwitchOffEvent();
}

void SleepController::update() {
    bool v8 = StageSwitchFunction::isOnSwitchBySwitchIdInfo(*_0);
    if (!_8 && v8) {
        _4->listenSwitchOnEvent();
    }
    if (_8 && !v8) {
        _4->listenSwitchOffEvent();
    }
    _8 = v8;
}
