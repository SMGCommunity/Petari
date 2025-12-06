#pragma once

#include "Game/Map/StageSwitch.hpp"
#include "Game/NameObj/NameObj.hpp"

class SwitchSynchronizer : public NameObj {
public:
    SwitchSynchronizer(const char*);

    virtual ~SwitchSynchronizer();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    StageSwitchCtrl* mSwitchCtrl;  // 0x0C
    u8 _10;
};
