#pragma once

#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"

class AreaObj;

class AreaObjFollower : public BaseMatrixFollower {
public:
    AreaObjFollower(AreaObj*, const JMapInfoIter&);

    virtual void update();

    /* 0x14 */ TPos3f mFollowMtx;
    /* 0x44 */ AreaObj* mObj;
};

namespace MR {
    bool addBaseMatrixFollowerAreaObj(AreaObj*, const JMapInfoIter&);
};  // namespace MR
