#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <revolution.h>
#include <JSystem/JParticle/JPAEmitterManager.hpp>

class AutoEffectGroupHolder;
class ParticleEmitter;
class SingleEmitter;
class ParticleCalcExecutor;
class ParticleDrawExecutor;
class ParticleEmitterHolder;

class EffectSystem : public NameObj {
public:
    EffectSystem(const char *, bool);

    ParticleEmitter* createEmitter(u16, u8, u8);
    void forceDeleteEmitter(ParticleEmitter *) const;
    void forceDeleteSingleEmitter(SingleEmitter *) const;

    u32 _C;
    ParticleEmitterHolder* mEmitterHolder;      // _10
    ParticleDrawExecutor* mDrawExec;            // _14
    ParticleCalcExecutor* mCalcExec;            // _18
    AutoEffectGroupHolder* mGroupHolder;        // _1C
};