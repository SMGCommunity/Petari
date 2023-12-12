#pragma once

#include "JSystem/JGeometry.h"

class NameObj;

// Gravity vector calculation flags
#define GRAVITY_TYPE_NORMAL 1
#define GRAVITY_TYPE_SHADOW 2
#define GRAVITY_TYPE_MAGNET 4
#define GRAVITY_TYPE_MARIO_LAUNCHER 8

// Gravity power types
#define GRAVITY_POWER_LIGHT 0
#define GRAVITY_POWER_NORMAL 1
#define GRAVITY_POWER_HEAVY 2

class PlanetGravity {
public:
	PlanetGravity();

	virtual void updateMtx(const TPos3f &rMtx) {}

	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const {
		return false;
	}

	bool calcGravity(TVec3f *pDest, const TVec3f &rPosition) const;
	bool calcGravityFromMassPosition(TVec3f *pDirection, f32 *pScalar, const TVec3f &rPosition, const TVec3f &rMassPosition) const;

	bool isInRangeSquare(f32 radius) const;
	bool isInRangeDistance(f32 radius) const;

	void setPriority(s32 priority);
	void updateIdentityMtx();

	f32 mRange;          // _4
	f32 mDistant;        // _8
	s32 mPriority;       // _C
	s32 mGravityId;      // _10, read from the stage files but never used
	const void* mHost;   // _14, host object pointer, can be any pointer
	u32 mGravityType;    // _18, gravity type bits
	s32 mGravityPower;   // _1C, gravity power type
	bool mActivated;     // _20, corresponding GlobalGravityObj has switch activated?
	bool mIsInverse;     // _21, invert gravity vector?
	bool mValidFollower; // _22
	bool mIsRegistered;  // _23, is registered by PlanetGravityManager?
	bool mAppeared;      // _24, corresponding GlobalGravityObj has appeared?
};
