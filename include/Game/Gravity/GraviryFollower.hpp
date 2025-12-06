#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"

class GlobalGravityObj;

class GraviryFollower : public BaseMatrixFollower {
public:
    GraviryFollower(GlobalGravityObj* pGravityObj, const JMapInfoIter& rIter);

    virtual void setGravityFollowHost(const NameObj* pHost);
    virtual void update();

    PlanetGravity* mGravity;  // 0x14
};

namespace MR {
    bool addBaseMatrixFollowerGravity(GlobalGravityObj* pGravityObj, const JMapInfoIter& rIter);
};
