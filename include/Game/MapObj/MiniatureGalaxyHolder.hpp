#pragma once

#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "revolution/types.h"
#include <revolution.h>

class MiniatureGalaxyFunction {
public:
    static s32 getMiniatureGalaxyNum();
    static MiniatureGalaxy* getPointingMiniatureGalaxy();
    static MiniatureGalaxy* getCometLandMiniatureGalaxy();
};
