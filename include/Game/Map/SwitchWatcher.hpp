#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Map/StageSwitch.hpp"

class SwitchWatcher : public NameObj {
public:
    SwitchWatcher(const StageSwitchCtrl *);

    virtual ~SwitchWatcher();
    virtual void movement();

    u32 mFlags;                                 // 0xC
    StageSwitchCtrl* mSwitchCtrl;               // 0x10
    SwitchEventListener* mSwitchAListener;      // 0x14
    SwitchEventListener* mSwitchBListener;      // 0x18
    SwitchEventListener* mSwitchApprListener;   // 0x1C
};