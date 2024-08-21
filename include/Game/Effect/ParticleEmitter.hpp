#pragma once

#include "JSystem/JParticle/JPAEmitterManager.hpp"
#include <revolution.h>

class ParticleEmitter {
public:
    ParticleEmitter();

    void invalidate();
    void init(u16);
    void pauseOn();
    void pauseOff();
    bool isContinuousParticle() const;

    JPAEmitterManager* mManager;    // 0x0
    bool mPaused;                   // 0x4
    bool mStopped;                  // 0x5 Not sure if it is stopped, but good chance it is.
};