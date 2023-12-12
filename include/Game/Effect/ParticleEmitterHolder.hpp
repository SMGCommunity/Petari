#pragma once

#include "Game/Effect/ParticleEmitter.h"
#include "Game/Util/Array.h"

class EffectSystem;

class ParticleEmitterHolder {
public:
    ParticleEmitterHolder(EffectSystem const *, int);

    EffectSystem const* mEffectSystem;              // _0
    MR::AssignableArray<ParticleEmitter> mEmitters; // _4
    int mNumEmitters;                               // _8
};