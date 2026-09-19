#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SeesawMoveNut : public MapObjActor {
public:
    SeesawMoveNut(const char*);

    virtual void init(const JMapInfoIter&);

    void exeHalfway();
    void exeEdge();
    void autoBackHome();
    bool isEnableStart() const;
    bool isAtEdge() const;

    /* 0xC4 */ f32 mSpeed;
    /* 0xC8 */ f32 mPrevAngularSpeed;
    /* 0xCC */ s32 _CC;
};
