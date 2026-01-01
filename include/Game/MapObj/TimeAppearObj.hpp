#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class TimeAppearObj : public MapObjActor {
public:
    TimeAppearObj(const char*);

    virtual ~TimeAppearObj();
    virtual void init(const JMapInfoIter&);
    virtual void appear();

    void exeHide();
    void exeEnd();

    s32 mTimer;  // 0xC4
};
