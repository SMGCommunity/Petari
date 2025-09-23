#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RushEndInfo {
public:
    RushEndInfo(LiveActor *, u32, const TVec3f &, bool, u32);
    u32 _0;
    u32 _4;
    TVec3f _8;
    bool _14;
    u32 _18;
    LiveActor* _1C;
    u32 _20;
};
