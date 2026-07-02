#pragma once

#include <revolution/types.h>

class JMapInfoIter;
class SwitchEventListener;
class SwitchIdInfo;

class SleepController {
public:
    SleepController(const JMapInfoIter&, SwitchEventListener*);
    void initSync();
    void update();

    SwitchIdInfo* _0;
    SwitchEventListener* _4;
    bool _8;
};
