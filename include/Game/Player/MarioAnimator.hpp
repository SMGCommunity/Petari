#pragma once

#include "Game/Player/MarioModule.hpp"

class MarioActor;

class MarioAnimator : public MarioModule {
public:
    MarioAnimator(MarioActor *);

    void setHoming();
    bool isAnimationStop();
    void setSpeed(f32);
    void forceSetBlendWeight(const f32 *);
    void waterToGround();

    XanimeResourceTable* _8;
    XanimePlayer* _C;
    XanimePlayer* _10;
};
