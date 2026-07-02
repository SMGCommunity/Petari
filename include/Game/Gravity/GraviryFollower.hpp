#pragma once

#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"

class GlobalGravityObj;
class PlanetGravity;

class GraviryFollower : public BaseMatrixFollower {
public:
    GraviryFollower(GlobalGravityObj* pGravityObj, const JMapInfoIter& rIter);

    virtual void setGravityFollowHost(const NameObj* pHost);
    virtual void update();

    /* 0x14 */ PlanetGravity* mGravity;
};

namespace MR {
    bool addBaseMatrixFollowerGravity(GlobalGravityObj* pGravityObj, const JMapInfoIter& rIter);
};  // namespace MR
