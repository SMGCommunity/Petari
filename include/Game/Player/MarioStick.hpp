#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;
class Traingle;

class MarioStick : public MarioState {
public:
    MarioStick(MarioActor *);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool postureCtrl(MtxPtr);

    bool startJump();
    bool setStickSensor(const HitSensor *);

    TVec3f _14;
    TVec3f _20;
    TVec3f _2C;
    TVec3f _38;
    f32 _44;
    Triangle* _48;
    u32 _4C;
    bool _50;
    bool _51;
    TVec3f _54;
    TVec3f _60;
    TVec3f _6C;
    TVec3f _78;
};
