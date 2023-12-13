#pragma once

#include "Game/Effect/ParticleEmitter.hpp"
#include "Game/Util/Array.hpp"

class EffectSystem;

class ParticleEmitterHolder {
public:
    ParticleEmitterHolder(EffectSystem const *, int);

    EffectSystem const* mEffectSystem;              // _0
    MR::AssignableArray<ParticleEmitter> mEmitters; // _4
    int mNumEmitters;                               // _8
};