#pragma once

#include "Game/Gravity/PlanetGravity.h"
#include "Game/Util/Array.h"

class WireGravity : public PlanetGravity {
public:
	WireGravity();

	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setPointListSize(u32);
	void addPoint(const TVec3f &);

	MR::AssignableArray<TVec3f> mPoints; // _28
	s32 mCount; // _30
};
