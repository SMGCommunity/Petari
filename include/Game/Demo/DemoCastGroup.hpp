#pragma once

#include "Game/NameObj/NameObj.hpp"

class JMapIdInfo;
class LiveActor;
class LiveActorGroup;

class DemoCastGroup : public NameObj {
public:
    DemoCastGroup(const char*);

    virtual ~DemoCastGroup() {
    }

    virtual void init(const JMapInfoIter&);
    virtual bool tryRegisterDemoActor(LiveActor*, const JMapInfoIter&, const JMapIdInfo&);
    virtual bool tryRegisterDemoActor(LiveActor*, const char*, const JMapInfoIter&);
    virtual void registerDemoActor(LiveActor*, const JMapInfoIter&);

    /* 0x0C */ JMapIdInfo* mInfo;
    /* 0x10 */ LiveActorGroup* mGroup;
};
