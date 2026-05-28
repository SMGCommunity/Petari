#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class IceMerameraKing;
class AnimScaleController;
class WalkerStateBindStarPointer;
class ParabolicPath;

class Meramera : public LiveActor {
public:
    Meramera(const char*);

    AnimScaleController* _8C; // 0x8C
    WalkerStateBindStarPointer* _90; // 0x90
    ParabolicPath* _94; // 0x94
    TPos3f _98; // 0x98
    TPos3f _C8; // 0xC8
    TPos3f _F8; // 0xF8
    TQuat4f _128; // 0x128
    TVec3f _138; // 0x138
    TVec3f _144; // 0x144
    TVec3f _150; // 0x150
    TVec3f _15C; // 0x15C
    TVec3f _168; // 0x168
    TVec3f _174; // 0x174
    TVec3f _180; // 0x180
    TVec3f _18C; // 0x18C
    TVec3f _198; // 0x198
    f32 _1A4; // 0x1A4
    f32 _1A8; // 0x1A8
    s32 mInitialBehaviour; // 0x1AC : Obj_arg0
    s32 _1B0; // 0x1B0
    s32 _1B4; // 0x1B4
    s32 _1B8; // 0x1B8
    s32 _1BC; // 0x1BC
    bool _1C0; // 0x1C0
    bool _1C1; // 0x1C1 : Unused?
    bool mRespawnEnable; // 0x1C2 : Obj_arg2
};
