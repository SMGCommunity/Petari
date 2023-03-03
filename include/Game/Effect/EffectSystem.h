#pragma once

#include <revolution.h>

class ParticleEmitter;
class SingleEmitter;

class EffectSystem {
public:
    EffectSystem(const char *, bool);

    ParticleEmitter* createEmitter(u16, u8, u8);
    void forceDeleteEmitter(ParticleEmitter *) const;
    void forceDeleteSingleEmitter(SingleEmitter *) const;
};