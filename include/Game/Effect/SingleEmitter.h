#pragma once

#include "Game/Effect/ParticleEmitter.h"

class EffectSystem;

enum SingleEmitterSpawn {
    SINGLE_EMITTER_ONE_TIME_SPAWN,
    SINGLE_EMITTER_CONTINUOUS_SPAWN,
    SINGLE_EMITTER_UNDEF_SPAWN
};

class SingleEmitter {
public:
    SingleEmitter();

    void init(u16);
    void deleteEmitter();
    void scanParticleEmitter(EffectSystem *);
    bool isOneTime() const;
    bool isValid() const;
    void link(ParticleEmitter *);
    void unlink();

    ParticleEmitter* mEmitter;  // _0
    u16 _4;                     // _4
    u8 mGroupId;                // _6
    s8 mSpawn;                  // _7
};