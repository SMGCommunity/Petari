#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaFireShort : public LiveActor {
public:
    KoopaFireShort(const Koopa*);

    void emitCurve();
    void emitNormal();
    void emitFast();
    void emitLongTime();

private:
    /* 0x8C */ Koopa* mKoopa;
    /* 0x90 */ f32 _90;
    /* 0x94 */ f32 _94;
    /* 0x98 */ f32 _98;
    /* 0x9C */ f32 _9C;
    /* 0xA0 */ f32 _A0;
    /* 0xA4 */ f32 _A4;
};
