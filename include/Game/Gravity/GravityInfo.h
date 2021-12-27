#pragma once

#include <revolution.h>
#include "Game/Gravity/PlanetGravity.h"

class GravityInfo {
public:
	GravityInfo();

	void init();

	TVec3f _0;
	s32 _C;
	PlanetGravity* mGravity; // _10
};
