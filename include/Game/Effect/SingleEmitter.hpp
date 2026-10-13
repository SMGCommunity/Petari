#pragma once

#include "Game/Effect/ParticleEmitter.hpp"

class EffectSystem;

enum SingleEmitterSpawn {
    /* 0x0 */ SINGLE_EMITTER_ONE_TIME_SPAWN,
    /* 0x1 */ SINGLE_EMITTER_CONTINUOUS_SPAWN,
    /* 0x2 */ SINGLE_EMITTER_UNDEF_SPAWN,
};

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

    void setGroupID(u8);

    /* 0x0 */ ParticleEmitter* mEmitter;
    /* 0x4 */ u16 _4;
    /* 0x6 */ u8 mGroupId;
    /* 0x7 */ s8 mSpawn;
};
