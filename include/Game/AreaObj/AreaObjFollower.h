#pragma once

#include "Game/AreaObj/AreaObj.h"
#include "Game/Util/BaseMatrixFollowTargetHolder.h"
#include "Game/Util.h"

class AreaObjFollower : public BaseMatrixFollower {
public:
    AreaObjFollower(AreaObj *, const JMapInfoIter &);

    virtual void update();

    TPos3f mFollowMtx;      // _14
    AreaObj* mObj;          // _44
};

namespace MR {
    bool addBaseMatrixFollowerAreaObj(AreaObj *, const JMapInfoIter &);
};
