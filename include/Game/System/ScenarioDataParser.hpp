#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <revolution.h>

class GalaxyStatusAccessor;

class ScenarioDataIter {
public:
    bool isEnd() const;
    void goNext();

    JMapInfo* _0;
    u32 mCur;   // 0x4
};

class ScenarioData {
public:
    ScenarioData(const char *);
    
    s32 getScenarioNum() const;
    s32 getPowerStarNum() const;
    bool getValueString(const char *, s32, const char **) const;
    const char* getZoneName(s32) const;
    ScenarioDataIter getScenarioDataIter(s32) const;
    u32 getValueU32(const char *, s32, u32 *) const;
    bool getValueBool(const char *, s32, bool *) const;
    s32 getZoneNum() const;
    s32 getZoneId(const char *) const;

    JMapInfo* mScenarioData; // 0x0
    const char* mGalaxyName; // 0x4
    JMapInfo* mZoneList;     // 0x8
};

class ScenarioDataParser : public NameObj {
public:
    ScenarioDataParser(const char *);

    virtual ~ScenarioDataParser();
    
    GalaxyStatusAccessor makeAccessor(const char *) const;

    static const char* sNullStr;
};

namespace ScenarioDataFunction {
    ScenarioDataParser getScenarioDataParser();

    u32 getCurrentCommonLayers(const char *);
    u32 getCurrentScenarioLayers(const char *, s32);
};

namespace MR {
    ScenarioDataIter makeBeginScenarioDataIter();
};