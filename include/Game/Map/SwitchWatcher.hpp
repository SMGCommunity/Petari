#pragma once

#include "Game/NameObj/NameObj.hpp"

class StageSwitchCtrl;
class SwitchEventListener;

class SwitchWatcher : public NameObj {
public:
    SwitchWatcher(const StageSwitchCtrl*);

    virtual void movement();

    void checkSwitch(SwitchEventListener*, u32, bool);
    bool isSameSwitch(const StageSwitchCtrl*) const;
    void addSwitchListener(SwitchEventListener*, u32);

    /* 0x0C */ u32 mFlags;
    /* 0x10 */ const StageSwitchCtrl* mSwitchCtrl;
    /* 0x14 */ SwitchEventListener* mSwitchListenerA;
    /* 0x18 */ SwitchEventListener* mSwitchListenerB;
    /* 0x1C */ SwitchEventListener* mSwitchListenerAppear;
};
