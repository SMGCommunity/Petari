#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class AnimationRandomPlayer : public NerveExecutor {
public:
    AnimationRandomPlayer(const LiveActor*, const char*, const char*, s32, f32);

    virtual ~AnimationRandomPlayer();

    void updateStartStep();
    void exeWait();
    void exePlay();

    const LiveActor* mActor;  // 0x8
    const char* _C;
    const char* _10;
    s32 _14;
    s32 _18;
    f32 _1C;
};
