#pragma once

#include <revolution.h>

class ScenarioData
{
public:
    ScenarioData(const char *);

    s32 getScenarioNum() const;
    s32 getPowerStarNum() const;
    bool getValueString(const char *, s32, const char **);
    const char* getZoneName(s32) const;
};