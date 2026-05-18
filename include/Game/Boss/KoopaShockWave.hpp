#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaShockWave : public LiveActor {
public:
    KoopaShockWave(Koopa*);

private:
    /* 0x8C */ Koopa* mKoopa;
    /* 0x90 */ f32 _90;
    /* 0x94 */ TMtx34f _94;
    /* 0xC4 */ f32 _C4;
    /* 0xC8 */ f32 _C8;
    /* 0xCC */ f32 _CC;
    /* 0xD0 */ f32 _D0;
    /* 0xD4 */ f32 _D4;
    /* 0xD8 */ f32 _D8;
    /* 0xDC */ TMtx34f _DC;
    /* 0x10C */ TMtx34f _10C;
    /* 0x13C */ f32 _13C;
    /* 0x140 */ f32 _140;
};
