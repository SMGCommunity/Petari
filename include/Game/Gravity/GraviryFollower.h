#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Util/BaseMatrixFollowTargetHolder.h"

class GlobalGravityObj;

class GraviryFollower : public BaseMatrixFollower {
public:
	GraviryFollower(GlobalGravityObj *pGravityObj, const JMapInfoIter &rIter);

	virtual void setGravityFollowHost(const NameObj *pHost);
	virtual void update();

	PlanetGravity* mGravity; // _14
};

namespace MR {
	bool addBaseMatrixFollowerGravity(GlobalGravityObj *pGravityObj, const JMapInfoIter &rIter);
};
