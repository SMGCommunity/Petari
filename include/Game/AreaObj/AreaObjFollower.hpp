#pragma once

#include "Game/AreaObj/AreaObj.hpp"
#include "Game/Util.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"

class AreaObjFollower : public BaseMatrixFollower {
public:
    AreaObjFollower(AreaObj*, const JMapInfoIter&);

    virtual void update();

    TPos3f mFollowMtx;  // 0x14
    AreaObj* mObj;      // 0x44
};

namespace MR {
    bool addBaseMatrixFollowerAreaObj(AreaObj*, const JMapInfoIter&);
};
