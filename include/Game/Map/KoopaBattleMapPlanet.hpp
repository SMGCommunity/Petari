#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class KoopaBattleMapPlanet : public LiveActor {
public:
    KoopaBattleMapPlanet(const char*, const char*, bool, bool, bool);

    void startAnimPowerStarAppear();

private:
    /* 0x8C */ const char* _8C;
    /* 0x90 */ TMtx34f _90;
    /* 0xC0 */ f32 _C0;
    /* 0xC4 */ f32 _C4;
    /* 0xC8 */ f32 _C8;
    /* 0xCC */ u32 _CC;
    /* 0xD0 */ u32 _D0;
    /* 0xD4 */ u32 _D4;
    /* 0xD8 */ u32 _D8;
    /* 0xDC */ bool _DC;
    /* 0xDD */ bool _DD;
    /* 0xDE */ bool _DE;
};
