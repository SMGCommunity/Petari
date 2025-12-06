#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class JPABaseEmitter;

class ParticleEmitter {
public:
    ParticleEmitter();

    void invalidate();
    void init(u16);
    void pauseOn();
    void pauseOff();
    bool isValid() const;
    bool isContinuousParticle() const;
    void setGlobalRotation(const TVec3s&);
    void setGlobalScale(const TVec3f&);
    void setGlobalSRTMatrix(const MtxPtr);
    void setGlobalPrmColor(u8, u8, u8);
    void setGlobalEnvColor(u8, u8, u8);

    /* 0x0 */ JPABaseEmitter* mEmitter;
    /* 0x4 */ bool mPaused;
    /* 0x5 */ bool mStopped;  // Not sure if it is stopped, but good chance it is.
};
