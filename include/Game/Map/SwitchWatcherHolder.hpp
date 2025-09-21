#pragma once

#include "Game/Map/StageSwitch.hpp"
#include "Game/NameObj/NameObj.hpp"

class SwitchEventListener;

// Might be derived from a class derived from NameObj
// Very incomplete: missing member functions + variables
class SwitchWatcherHolder : public NameObj {
public:
    SwitchWatcherHolder();

    virtual ~SwitchWatcherHolder();

    void joinSwitchEventListenerAppear(const StageSwitchCtrl *, SwitchEventListener *);
};

namespace MR {
    SwitchWatcherHolder* getSwitchWatcherHolder();
};
