#pragma once

#include "Game/Gravity/PlanetGravity.hpp"
#include <revolution.h>

class GravityInfo {
public:
    GravityInfo();

    void init();

    /* 0x00 */ TVec3f mGravityVector;
    /* 0x0C */ s32 mLargestPriority;
    /* 0x10 */ PlanetGravity* mGravityInstance;
};
