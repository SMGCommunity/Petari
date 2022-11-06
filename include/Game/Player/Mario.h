#pragma once

#include "Game/Player/MarioModule.h"

class MarioActor;
class Triangle;

class Mario : public MarioModule {
public:
    Mario(MarioActor *);

    Mtx _64;
    Mtx _94;
    Mtx _C4;

    TVec3f _130;

    TVec3f _160;

    TVec3f _184;
    TVec3f _190;
    TVec3f _19C;
    TVec3f _1A8;

    TVec3f _1CC;

    TVec3f _238;
    TVec3f _244;
    TVec3f _250;

    TVec3f _25C;
    TVec3f _268;

    TVec3f _350;
    TVec3f _35C;

    TVec3f _3D8;

    TVec3f _43C;

    Triangle* _45C;
    Triangle* _460;
    Triangle* _464;
    Triangle* _468;
    Triangle* _46C;
    Triangle* _470;
    Triangle* _474;
    Triangle* _478;
    Triangle* _47C;
    Triangle* _480;
    Triangle* _484;

    TVec3f _4A4;
    TVec3f _4B0;

    Triangle* _4C8;
    Triangle* _4CC;
    Triangle* _4D0;
    Triangle* _4D4;
    Triangle* _4D8;
    Triangle* _4DC;

    TVec3f _4E8;
    TVec3f _4F4;

    Triangle* _57C;

    TVec3f _6A0;

    TVec3f _6B0;
    TVec3f _6BC;
    
    TVec3f _7D4;
    Triangle* _7E0;

    Triangle* _820;

    TVec3f _8A4;

    Triangle* _8C8;
    Triangle* _8CC;
    Triangle* _8D0;

    Triangle* _8E8;

    TVec3f _910;
    TVec3f _91C;
    TVec3f _928;

    TVec3f _A8C;
};