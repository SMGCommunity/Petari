#pragma once

#include "Game/NameObj/NameObj.h"

class GlobalGravityObj;
class GraviryFollower;

namespace MR {
	void addBaseMatrixFollowerGravity(GlobalGravityObj *pGravityObj, const JMapInfoIter &rIter);
};
