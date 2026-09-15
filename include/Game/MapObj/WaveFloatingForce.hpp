#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class WaveFloatingForce {
public:
    WaveFloatingForce(const LiveActor*, s32, f32, bool);

    void update();
    f32 getCurrentValue() const;
    void updateNormal();
    void updateOnPlayer();

    /* 0x00 */ const LiveActor* mActor;
    /* 0x04 */ s32 mStepCount;
    /* 0x08 */ f32 mAmplitude;
    /* 0x0C */ f32 mStep;
    /* 0x10 */ bool _10;
};
