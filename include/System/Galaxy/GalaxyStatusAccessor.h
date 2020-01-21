#ifndef GALAXYSTATUSACCESSOR_H
#define GALAXYSTATUSACCESSOR_H

#include "types.h"

class GalaxyStatusAccessor;

namespace MR
{
    static GalaxyStatusAccessor* makeGalaxyStatusAcessor(const char *);
    static GalaxyStatusAccessor makeCurrentGalaxyStatusAccessor();
};

class GalaxyStatusAccessor
{
public:
    u32 getScenarioNum() const;
    u32 getPowerStarNum() const;
    u32 getZoneNum() const;
    u32 getZoneId(const char *) const;
    const char* getZoneName(s32) const;
    const char* getCometName(s32) const;
};

#endif // GALAXYSTATUSACCESSOR_H