#pragma once

#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/NameObj/NameObjGroup.hpp"
#include "Game/Util.hpp"

class DemoCastGroupHolder : public NameObjGroup {
public:
    DemoCastGroupHolder();

    virtual ~DemoCastGroupHolder();

    bool tryRegisterDemoActor(LiveActor*, const JMapInfoIter&, const JMapIdInfo&) const;
    bool tryRegisterDemoActor(LiveActor*, const char*, const JMapInfoIter&) const;
    DemoCastGroup* getCastGroup(int) const;
    DemoCastGroup* findCastGroup(const char*) const;
};