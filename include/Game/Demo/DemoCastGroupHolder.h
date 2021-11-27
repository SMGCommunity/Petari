#pragma once

#include "Game/NameObj/NameObjGroup.h"
#include "Game/Demo/DemoCastGroup.h"
#include "Game/Util.h"

class DemoCastGroupHolder : public NameObjGroup {
public:
    DemoCastGroupHolder();

    virtual ~DemoCastGroupHolder();

    bool tryRegisterDemoActor(LiveActor *, const JMapInfoIter &, const JMapIdInfo &) const;
    bool tryRegisterDemoActor(LiveActor *, const char *, const JMapInfoIter &) const;
    DemoCastGroup* getCastGroup(int) const;
    DemoCastGroup* findCastGroup(const char *) const;
};