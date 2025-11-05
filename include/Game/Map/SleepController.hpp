#pragma once

#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Util/JMapInfo.hpp"

class SleepController {
public:
    SleepController(const JMapInfoIter&, SwitchEventListener*);
    void initSync();
    void update();

    SwitchIdInfo* _0;
    SwitchEventListener* _4;
    bool _8;
};
