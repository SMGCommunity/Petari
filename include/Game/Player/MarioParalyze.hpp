#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioParalyze : public MarioState {
public:
    MarioParalyze(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    /* 0x12 */ u16 _12;
    /* 0x14 */ u16 mTimer;
    /* 0x16 */ u16 _16;
    /* 0x18 */ bool mNotDecLife;
};
