#pragma once

#include <revolution.h>

class KCollisionServer;

class CollisionParts
{
public:
    CollisionParts(); 

    void updateMtx();   

    u32 _0;
    Mtx _4;
    Mtx _34;
    Mtx _64;
    Mtx _94;
    KCollisionServer* mCollisionServer; // _C4
    u32 _C8;
    u8 _CC;
    u8 _CD;
    u8 _CE;
    u8 _CF;
    u8 _D0;
    u32 _D4;
    f32 _D8;
    f32 _DC;
    s32 _E0;
    u32 _E4;
};