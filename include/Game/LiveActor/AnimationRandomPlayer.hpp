#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class AnimationRandomPlayer : public NerveExecutor {
public:
    AnimationRandomPlayer(const LiveActor *, const char *, const char *, s32, f32);

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

namespace NrvAnimationRandomPlayer {
    NERVE(HostTypeWait);
    NERVE(HostTypePlay);

    void HostTypePlay::execute(Spine *pSpine) const {
        AnimationRandomPlayer* player = reinterpret_cast<AnimationRandomPlayer*>(pSpine->mExecutor);
        player->exePlay();
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        AnimationRandomPlayer* player = reinterpret_cast<AnimationRandomPlayer*>(pSpine->mExecutor);
        player->exeWait();
    }
};