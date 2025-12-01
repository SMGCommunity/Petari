#pragma once

#include "Game/Map/SwitchWatcher.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class StageSwitchCtrl;
class SwitchEventListener;

class SwitchWatcherHolder : public NameObj {
public:
    /// @brief Creates a new `SwitchWatcherHolder`.
    SwitchWatcherHolder();

    virtual ~SwitchWatcherHolder();
    virtual void movement();

    void joinSwitchEventListenerA(const StageSwitchCtrl*, SwitchEventListener*);
    void joinSwitchEventListenerB(const StageSwitchCtrl*, SwitchEventListener*);
    void joinSwitchEventListenerAppear(const StageSwitchCtrl*, SwitchEventListener*);
    SwitchWatcher* findSwitchWatcher(const StageSwitchCtrl*);
    void joinSwitchEventListener(const StageSwitchCtrl*, u32, SwitchEventListener*);
    void addSwitchWatcher(SwitchWatcher*);

    MR::FixedArray< SwitchWatcher*, 256 > mWatchers;  // _C
    s32 mWatcherCount;                                // _40C
};

namespace MR {
    SwitchWatcherHolder* getSwitchWatcherHolder();
    void requestMovementOnSwitchWatcher();
};  // namespace MR
