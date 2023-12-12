#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/LiveActorGroup.h"
#include "Game/Util.h"

class LiveActor;

class DemoCastGroup : public NameObj {
public:
    DemoCastGroup(const char *);

    virtual ~DemoCastGroup();

    virtual void init(const JMapInfoIter &);
    virtual bool tryRegisterDemoActor(LiveActor *, const JMapInfoIter &, const JMapIdInfo &);
    virtual bool tryRegisterDemoActor(LiveActor *, const char *, const JMapInfoIter &);
    virtual void registerDemoActor(LiveActor *, const JMapInfoIter &);

    JMapIdInfo* mInfo;      // _C
    LiveActorGroup* mGroup; // _10
};