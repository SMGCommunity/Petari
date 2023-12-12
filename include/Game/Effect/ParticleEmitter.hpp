#pragma once

#include "JSystem/JParticle/JPAEmitterManager.h"
#include <revolution.h>

class ParticleEmitter {
public:
    ParticleEmitter();

    void invalidate();
    void init(u16);
    void pauseOn();
    void pauseOff();
    bool isContinuousParticle() const;

    JPAEmitterManager* mManager;    // _0
    bool mPaused;                   // _4
    bool mStopped;                  // _5 Not sure if it is stopped, but good chance it is.
};