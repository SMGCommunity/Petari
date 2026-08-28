#pragma once

#include "Game/NameObj/NameObj.hpp"

class StageSwitchCtrl;

class SwitchSynchronizer : public NameObj {
public:
    SwitchSynchronizer(const char*);

    /* 0x08 */ virtual ~SwitchSynchronizer();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    StageSwitchCtrl* mSwitchCtrl;  // 0x0C
    bool _10;
};
