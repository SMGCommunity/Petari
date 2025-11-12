#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class StageSwitchCtrl;
class SwitchEventListener;
class SwitchWatcher;

class SwitchWatcherHolder : public NameObj {
public:
    /// @brief Creates a new `SwitchWatcherHolder`.
    SwitchWatcherHolder();

    virtual void movement();

    void joinSwitchEventListenerA(const StageSwitchCtrl*, SwitchEventListener*);
    void joinSwitchEventListenerB(const StageSwitchCtrl*, SwitchEventListener*);
    void joinSwitchEventListenerAppear(const StageSwitchCtrl*, SwitchEventListener*);
    SwitchWatcher* findSwitchWatcher(const StageSwitchCtrl*);
    void joinSwitchEventListener(const StageSwitchCtrl*, u32, SwitchEventListener*);
    void addSwitchWatcher(SwitchWatcher*);

    /* 0x0C */ MR::Vector<MR::FixedArray<SwitchWatcher*, 256> > mSwitchWatcher;
};

namespace MR {
    SwitchWatcherHolder* getSwitchWatcherHolder();
    void requestMovementOnSwitchWatcher();
};
