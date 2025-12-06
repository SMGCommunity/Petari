#pragma once

#include "Game/Effect/ParticleEmitter.hpp"

class EffectSystem;

enum SingleEmitterSpawn { SINGLE_EMITTER_ONE_TIME_SPAWN, SINGLE_EMITTER_CONTINUOUS_SPAWN, SINGLE_EMITTER_UNDEF_SPAWN };

class SingleEmitter {
public:
    SingleEmitter();

    void init(u16);
    void deleteEmitter();
    void scanParticleEmitter(EffectSystem*);
    bool isOneTime() const;
    bool isValid() const;
    void link(ParticleEmitter*);
    void unlink();

    ParticleEmitter* mEmitter;  // 0x0
    u16 _4;                     // 0x4
    u8 mGroupId;                // 0x6
    s8 mSpawn;                  // 0x7
};