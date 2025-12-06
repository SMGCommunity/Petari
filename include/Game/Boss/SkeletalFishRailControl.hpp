#pragma once

#include "Game/LiveActor/LiveActor.hpp"

struct SkeletalFishRailInfo {
    LiveActor* _0;
    f32 _4;
};

class SkeletalFishRailControl {
public:
    SkeletalFishRailControl();

    void setRailActor(LiveActor*, LiveActor*, bool);
    void update();
    void getRailInfo(SkeletalFishRailInfo*, f32);
    void getPos(TVec3f*, f32);
    void getMtx(TPos3f*, f32);

    u32 _0;
    LiveActor* _4;
    LiveActor* _8;
    f32 _C;
    f32 _10;
    f32 _14;
    bool _18;
};