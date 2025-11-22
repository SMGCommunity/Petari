#pragma once

#include "Game/Util/JMapInfo.hpp"

class SwitchIdInfo;
class SwitchEventListener;

class SleepController {
public:
    SleepController(const JMapInfoIter&, SwitchEventListener*);
    void initSync();
    void update();

    SwitchIdInfo* _0;
    SwitchEventListener* _4;
    bool _8;
};
