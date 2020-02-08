#ifndef GALAXYSTATUSACCESSOR_H
#define GALAXYSTATUSACCESSOR_H

#include <revolution.h>

class GalaxyStatusAccessor
{
public:
    u32 getScenarioNum() const;
    u32 getPowerStarNum() const;
    u32 getZoneNum() const;
    u32 getZoneId(const char *) const;
    const char* getZoneName(s32) const;
    const char* getCometName(s32) const;
    
    u32* _0; // ScenarioData*
};

namespace MR
{
    static GalaxyStatusAccessor* makeGalaxyStatusAcessor(const char *);
    static void* makeCurrentGalaxyStatusAccessor();
};

#endif // GALAXYSTATUSACCESSOR_H