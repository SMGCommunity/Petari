#pragma once

#include "Game/Player/MarioModule.hpp"

class MarioActor;

class MarioMove : public MarioModule {
public:
    MarioMove(MarioActor *);
    void initAfter();

    TVec3f _8;
    TVec3f _14;
    TVec3f _20;
    TVec3f _2C;
    TVec3f _38;
    TVec3f _44;
    float _50;
    float _54;
};
