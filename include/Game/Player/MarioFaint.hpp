#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFaint : public MarioState {
public:
    MarioFaint(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    void setVec(const TVec3f&);

    /* 0x12 */ u16 mTimer;
    /* 0x14 */ u16 _14;
    /* 0x16 */ u16 _16;
    /* 0x18 */ TVec3f _18;
    /* 0x24 */ bool mNoDamage;
    /* 0x25 */ bool mTookDamage;
};
