#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class TimeAppearObj : public MapObjActor {
public:
    TimeAppearObj(const char *);

    virtual ~TimeAppearObj();
    virtual void init(const JMapInfoIter &);
    virtual void appear();

    void exeEnd();

    s32 mTimer; // _C4
};

namespace NrvTimeAppearObj {
    NERVE(TimeAppearObjNrvHide);
    NERVE(TimeAppearObjNrvEnd);
};