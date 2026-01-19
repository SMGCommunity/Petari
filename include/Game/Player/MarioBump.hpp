#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioBump : public MarioState {
public:
    MarioBump(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    f32 _14;
    TVec3f _18;
};
