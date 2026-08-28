#pragma once

#include "Game/NameObj/NameObjGroup.hpp"

class DemoCastGroup;
class JMapIdInfo;
class LiveActor;

class DemoCastGroupHolder : public NameObjGroup {
public:
    DemoCastGroupHolder();

    /* 0x08 */ virtual ~DemoCastGroupHolder();

    bool tryRegisterDemoActor(LiveActor*, const JMapInfoIter&, const JMapIdInfo&) const;
    bool tryRegisterDemoActor(LiveActor*, const char*, const JMapInfoIter&) const;
    DemoCastGroup* getCastGroup(int) const;
    DemoCastGroup* findCastGroup(const char*) const;
};