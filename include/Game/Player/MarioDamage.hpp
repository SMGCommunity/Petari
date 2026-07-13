#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioDamage : public MarioState {
public:
    MarioDamage(MarioActor*);

    inline void setStrings(const char* pChar1, const char* pChar2) {
        _28 = pChar1;
        _2C = pChar2;
    }

    /* 0x11 */ u8 _11;
    /* 0x12 */ u8 _12;
    /* 0x14 */ u16 _14;
    /* 0x16 */ u16 _16;
    /* 0x18 */ u16 _18;
    /* 0x1C */ TVec3f _1C;
    /* 0x28 */ const char* _28;
    /* 0x2C */ const char* _2C;
};
