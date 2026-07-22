#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class TimeAppearObj : public MapObjActor {
public:
    TimeAppearObj(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();

    void exeHide();
    void exeEnd();

    /* 0xC4 */ s32 mTimer;
};
