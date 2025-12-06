#pragma once

#include "Game/Gravity/PlanetGravity.hpp"
#include <revolution.h>

class GravityInfo {
public:
    GravityInfo();

    void init();

    TVec3f mGravityVector;            // 0x0
    s32 mLargestPriority;             // 0xC
    PlanetGravity* mGravityInstance;  // 0x10
};
