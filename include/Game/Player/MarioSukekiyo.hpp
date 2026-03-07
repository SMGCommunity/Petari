#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSukekiyo : public MarioState {
public:
    MarioSukekiyo(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool postureCtrl(MtxPtr);

    TVec3f _14;
    TVec3f _20;
    TVec3f _2C;
    TVec3f _38;
    Triangle* _44;
    u16 _48;
    u8 _4A;
};

class MarioBury : public MarioSukekiyo {
public:
    MarioBury(MarioActor*);
};
