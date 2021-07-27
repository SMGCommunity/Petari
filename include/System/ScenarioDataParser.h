#pragma once

#include "NameObj/NameObj.h"
#include "JSystem/JMap/JMapInfo.h"
#include <revolution.h>

class GalaxyStatusAccessor;

class ScenarioData 
{
public:
    ScenarioData(const char *);
    
    s32 getScenarioNum() const;
    s32 getPowerStarNum() const;
    bool getValueString(const char *, s32, const char **) const;
    const char* getZoneName(s32) const;
    JMapInfo* getScenarioDataIter(s32) const;
    u32 getValueU32(const char *, s32, u32 *) const;
    bool getValueBool(const char *, s32, bool *);
    s32 getZoneNum() const;
    s32 getZoneId(const char *) const;

    JMapInfo *mScenarioInfo; // _0
    const char *_4; // possibly ScenarioName? 
    bool **_8;
};

class ScenarioDataParser : public NameObj
{
public:
    ScenarioDataParser(const char *);
    
    GalaxyStatusAccessor* makeAccessor(const char *) const;
};

namespace ScenarioDataFunction
{
    ScenarioDataParser* getScenarioDataParser();
};