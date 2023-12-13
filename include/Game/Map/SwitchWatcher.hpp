#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Map/StageSwitch.hpp"

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