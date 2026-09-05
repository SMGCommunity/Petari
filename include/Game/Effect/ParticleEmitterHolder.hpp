#pragma once

#include "Game/Effect/ParticleEmitter.hpp"
#include "Game/Util/Array.hpp"

class EffectSystem;

class ParticleEmitterHolder {
public:
    ParticleEmitterHolder(const EffectSystem*, int);

    void update(bool);
    void forceDeleteAllOneTimeEmitters();
    void forceDeleteAllEmitters();
    void requestMovementOnAllEmitters();
    ParticleEmitter* findAvailableParticleEmitter();
    void requestMovementOffAllLoopEmitters();

    /* 0x0 */ const EffectSystem* mEffectSystem;
    /* 0x4 */ MR::AssignableArray< ParticleEmitter > mEmitters;
};
