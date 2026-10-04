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

    /* 0x08 */ const LiveActor* mActor;
    /* 0x0C */ const char* _C;
    /* 0x10 */ const char* _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ s32 _18;
    /* 0x1C */ f32 _1C;
};
