#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <revolution.h>

const JMapData GalaxyIDBCSV = {0x0, 0x0, 0x0, 0x0};

namespace GameDataConst {
    s32 getPowerStarNumToOpenGalaxy(const char*);
    bool isPowerStarGreen(const char*, s32);
    bool isPowerStarRed(const char*, s32);
    bool isPowerStarLuigiHas(const char*, s32);
    bool isGrandStar(const char*, s32);
    bool isGalaxyLuigiArrested(const char*, s32);
    bool isQuestionGalaxy(const char*);
    bool isGalaxyAppearGreenDriver(const char*);
    u32 getIncludedGrandGalaxyId(const char*);
    bool isPowerStarSpecial(const char*, s32, const char*);
};  // namespace GameDataConst
