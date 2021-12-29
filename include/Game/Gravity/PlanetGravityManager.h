#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Gravity/GravityInfo.h"
#include "Game/Gravity/PlanetGravity.h"

class GravityObj;

class PlanetGravityManager : public NameObj {
public:
	PlanetGravityManager(const char *pName);

	virtual ~PlanetGravityManager() {}
	virtual void init(const JMapInfoIter &);

	bool calcTotalGravityVector(TVec3f *pDest, GravityInfo *pInfo, const TVec3f &rPosition, u32 gravityType, u32 host) const;
	void registerGravity(PlanetGravity *pGravity);

	PlanetGravity* mGravities[128]; // _C
	s32 mNumGravities; // _20C
};
