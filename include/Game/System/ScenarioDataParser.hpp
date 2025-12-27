#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class GalaxyStatusAccessor;
class JMapInfo;
class ScenarioData;

class ScenarioDataParser : public NameObj {
public:
    /// @brief Creates a new `ScenarioDataParser`.
    /// @param pName A pointer to the null-terminated name of the object.
    ScenarioDataParser(const char* pName);

    const ScenarioData* getScenarioData(const char*) const;
    const ScenarioData* getScenarioData(s32) const;
    GalaxyStatusAccessor makeAccessor(const char*) const;

    /* 0x0C */ MR::Vector< MR::FixedArray< ScenarioData*, 64 > > mScenarioData;
};

class ScenarioDataIter {
public:
    ScenarioDataIter(const ScenarioDataParser*, int);

    bool isEnd() const;
    void goNext();
    GalaxyStatusAccessor makeAccessor() const;

    /* 0x00 */ const ScenarioDataParser* mParser;
    /* 0x04 */ int mCur;
};

class ScenarioData {
public:
    ScenarioData(const char*);

    s32 getScenarioNum() const;
    s32 getPowerStarNum() const;
    bool getValueString(const char*, s32, const char**) const;
    const char* getZoneName(s32) const;
    ScenarioDataIter getScenarioDataIter(s32) const;
    bool getValueU32(const char*, s32, u32*) const;
    bool getValueBool(const char*, s32, bool*) const;
    s32 getZoneNum() const;
    s32 getZoneId(const char*) const;

    /* 0x00 */ JMapInfo* mScenarioData;
    /* 0x04 */ char* mGalaxyName;
    /* 0x08 */ JMapInfo* mZoneList;
};

namespace ScenarioDataFunction {
    ScenarioDataParser* getScenarioDataParser();
    u32 getCurrentCommonLayers(const char*);
    u32 getCurrentScenarioLayers(const char*, s32);
};  // namespace ScenarioDataFunction

namespace MR {
    ScenarioDataIter makeBeginScenarioDataIter();
};  // namespace MR
