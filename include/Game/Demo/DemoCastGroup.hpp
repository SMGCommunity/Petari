#pragma once

#include "Game/NameObj/NameObj.hpp"

class JMapIdInfo;
class LiveActor;
class LiveActorGroup;

class DemoCastGroup : public NameObj {
public:
    DemoCastGroup(const char*);

    /* 0x08 */ virtual ~DemoCastGroup() {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual bool tryRegisterDemoActor(LiveActor*, const JMapInfoIter&, const JMapIdInfo&);
    /* 0x24 */ virtual bool tryRegisterDemoActor(LiveActor*, const char*, const JMapInfoIter&);
    /* 0x28 */ virtual void registerDemoActor(LiveActor*, const JMapInfoIter&);

    JMapIdInfo* mInfo;       // 0xC
    LiveActorGroup* mGroup;  // 0x10
};
