#pragma once

#include "Game/Util.h"

class ScenarioData {
public:
    ScenarioData(const char *);

    JMapInfo* mScenarioInfo;    // _0
    char* mName;                // _4
    JMapInfo* mZoneListInfo;    // _8
};