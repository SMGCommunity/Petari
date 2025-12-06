#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util.hpp"

class LiveActor;

class DemoCastGroup : public NameObj {
public:
    DemoCastGroup(const char*);

    virtual ~DemoCastGroup() {}

    virtual void init(const JMapInfoIter&);
    virtual bool tryRegisterDemoActor(LiveActor*, const JMapInfoIter&, const JMapIdInfo&);
    virtual bool tryRegisterDemoActor(LiveActor*, const char*, const JMapInfoIter&);
    virtual void registerDemoActor(LiveActor*, const JMapInfoIter&);

    JMapIdInfo* mInfo;       // 0xC
    LiveActorGroup* mGroup;  // 0x10
};
