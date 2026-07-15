#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RushEndInfo {
public:
    RushEndInfo(LiveActor*, u32, const TVec3f&, bool, u32);
    
    /* 0x00 */ u32 _0;
    /* 0x04 */ u32 _4;
    /* 0x08 */ TVec3f _8;
    /* 0x14 */ bool _14;
    /* 0x18 */ u32 _18;
    /* 0x1C */ LiveActor* _1C;
    /* 0x20 */ u32 _20;
};
