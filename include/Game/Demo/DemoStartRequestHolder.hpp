#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util.hpp"

class DemoStartInfo {
public:
    DemoStartInfo();

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
};

class DemoStartRequestHolder {
public:
    DemoStartRequestHolder();

    DemoStartInfo* mStartInfos[0x10];                               // 0x0
    u32 _40;
    const DemoStartInfo* _44;
    u8 _48[0x84-0x48];
    MR::FixedRingBuffer<const DemoStartInfo*, 16>::iterator _84;
    MR::FixedRingBuffer<const DemoStartInfo*, 16>::iterator _94;
    u32 _9C;
    NameObj* _A0;
};