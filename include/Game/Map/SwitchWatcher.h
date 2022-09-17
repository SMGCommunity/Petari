#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Map/StageSwitch.h"

class SwitchWatcher : public NameObj {
public:
    SwitchWatcher(const StageSwitchCtrl *);

    virtual ~SwitchWatcher();
    virtual void movement();

    u32 mFlags;                                 // _C
    StageSwitchCtrl* mSwitchCtrl;               // _10
    SwitchEventListener* mSwitchAListener;      // _14
    SwitchEventListener* mSwitchBListener;      // _18
    SwitchEventListener* mSwitchApprListener;   // _1C
};