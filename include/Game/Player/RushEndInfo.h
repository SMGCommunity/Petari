#pragma once

#include "Game/LiveActor/LiveActor.h"

class RushEndInfo {
public:
	RushEndInfo(LiveActor *, unsigned long, const TVec3f &, bool, unsigned long);
	u8 _0[0x24];
};