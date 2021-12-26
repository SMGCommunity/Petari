#pragma once

#include <revolution.h>
#include "Game/Gravity/PlanetGravity.h"

class GravityInfo {
public:
	GravityInfo();

	void init();

	f32 _0;
	f32 _4;
	f32 _8;
	s32 _C;
	PlanetGravity* mGravity;
};
