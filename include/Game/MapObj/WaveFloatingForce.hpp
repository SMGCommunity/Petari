#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class WaveFloatingForce {
public:
    WaveFloatingForce(const LiveActor*, s32, f32, bool);

    void update();
    f32 getCurrentValue() const;
    void updateNormal();
    void updateOnPlayer();

    const LiveActor* mActor;  // 0x0
    s32 _4;
    f32 _8;
    f32 _C;
    bool _10;
};