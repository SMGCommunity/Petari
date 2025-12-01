#pragma once

#include "Game/Map/StageSwitch.hpp"
#include "Game/NameObj/NameObj.hpp"

class SwitchEventListener;

class SwitchWatcher : public NameObj {
public:
    SwitchWatcher(const StageSwitchCtrl*);

    virtual ~SwitchWatcher();
    virtual void movement();

    void checkSwitch(SwitchEventListener*, u32, bool);
    bool isSameSwitch(const StageSwitchCtrl*) const;
    void addSwitchListener(SwitchEventListener*, u32);

    u32 mFlags;                                // 0xC
    const StageSwitchCtrl* mSwitchCtrl;        // 0x10
    SwitchEventListener* mSwitchAListener;     // 0x14
    SwitchEventListener* mSwitchBListener;     // 0x18
    SwitchEventListener* mSwitchApprListener;  // 0x1C
};
