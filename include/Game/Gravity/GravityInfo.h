#pragma once

#include <revolution.h>
#include "Game/Gravity/PlanetGravity.h"

class GravityInfo {
public:
	GravityInfo();

	void init();

	TVec3f mGravityVector; // _0
	s32 mLargestPriority; // _C
	PlanetGravity* mGravityInstance; // _10
};
