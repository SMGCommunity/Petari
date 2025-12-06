#pragma once

#include "Game/Util.hpp"
#include "revolution.h"

class J3DModel;
class LiveActor;

class DrawBufferExecuter {
public:
    DrawBufferExecuter(const char*, J3DModel*, s32);

    void add(LiveActor*);

    void findLightInfo(LiveActor*);
    void onExecuteLight(s32);
    void offExecuteLight();

    void calcViewAndEntry();

    u8 _0[0x8];
    u32 _8;
    const char* mName;  // 0xC
    u32 _10;
    u32 _14;
    u32 mDrawBufferCount;  // 0x18
};