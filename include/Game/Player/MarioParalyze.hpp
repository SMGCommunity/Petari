#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioParalyze : public MarioState {
public:
    MarioParalyze(MarioActor *);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    u16 _12;
    u16 _14;
    u16 _16;
    bool _18;
};
