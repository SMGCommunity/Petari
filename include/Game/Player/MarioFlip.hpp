#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFlip : public MarioState {
public:
    MarioFlip(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x18 */ virtual bool close();
    /* 0x1C */ virtual bool update();

    void setVec(const TVec3f&);

    /* 0x12 */ u16 _12;
    /* 0x14 */ u16 _14;
    /* 0x18 */ TVec3f _18;
    /* 0x24 */ f32 _24;
    /* 0x28 */ f32 _28; 
};
