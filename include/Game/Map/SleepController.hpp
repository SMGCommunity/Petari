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

    /* 0x00 */ SwitchIdInfo* mSwitchIdInfo;
    /* 0x04 */ SwitchEventListener* mSwitchEventListener;
    /* 0x08 */ bool mIsOnSwitchByIdInfo;
};
