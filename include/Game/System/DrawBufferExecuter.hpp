#pragma once

#include "revolution.h"
#include "Game/Util.hpp"

class J3DModel;
class LiveActor;

class DrawBufferExecuter {
public:
    DrawBufferExecuter(const char *, J3DModel *, s32);

    void add(LiveActor *);

    void findLightInfo(LiveActor *);
    void onExecuteLight(s32);
    void offExecuteLight();

    void calcViewAndEntry();

    u8 _0[0x8];
    u32 _8;
    const char* mName; // _C
    u32 _10;
    u32 _14;
    u32 mDrawBufferCount; // _18
};