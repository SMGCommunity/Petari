#pragma once

#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/Demo/DemoExecutor.hpp"

class DemoCastSubGroup : public DemoCastGroup {
public:
    DemoCastSubGroup(const char*);

    /* 0x08 */ virtual ~DemoCastSubGroup();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual bool tryRegisterDemoActor(LiveActor*, const JMapInfoIter&, const JMapIdInfo&);

    DemoExecutor* mExecutor;  // 0x14
};
