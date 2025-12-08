#pragma once

#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/Demo/DemoExecutor.hpp"

class DemoCastSubGroup : public DemoCastGroup {
public:
    DemoCastSubGroup(const char*);

    virtual ~DemoCastSubGroup();
    virtual void init(const JMapInfoIter&);
    virtual bool tryRegisterDemoActor(LiveActor*, const JMapInfoIter&, const JMapIdInfo&);

    DemoExecutor* mExecutor;  // 0x14
};
