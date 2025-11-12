#pragma once

#include <revolution.h>
#include "Game/Util/JMapInfo.hpp"

const JMapData GalaxyIDBCSV = { 0x0, 0x0, 0x0, 0x0 };

class GameDataConst {
public:
    static s32 getPowerStarNumToOpenGalaxy(const char *);
    static bool isPowerStarGreen(const char *, s32);
    static bool isPowerStarRed(const char *, s32);
    static bool isPowerStarLuigiHas(const char *, s32);
    static bool isGrandStar(const char *, s32);
    static bool isGalaxyLuigiArrested(const char *, s32);
    static bool isQuestionGalaxy(const char *);
    static bool isGalaxyAppearGreenDriver(const char *);
    static u32 getIncludedGrandGalaxyId(const char *);
    static bool isPowerStarSpecial(const char *, s32, const char *);
};
