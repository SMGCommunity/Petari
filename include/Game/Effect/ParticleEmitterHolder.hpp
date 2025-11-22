#pragma once

#include "Game/Effect/ParticleEmitter.hpp"
#include "Game/Util/Array.hpp"

class EffectSystem;

class ParticleEmitterHolder {
public:
    ParticleEmitterHolder(EffectSystem const*, int);

    EffectSystem const* mEffectSystem;                 // 0x0
    MR::AssignableArray< ParticleEmitter > mEmitters;  // 0x4
    int mNumEmitters;                                  // 0x8
};