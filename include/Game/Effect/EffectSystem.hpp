#pragma once

#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <revolution.h>
#include "Game/NameObj/NameObj.hpp"

class AutoEffectGroupHolder;
class ParticleEmitter;
class SingleEmitter;
class ParticleCalcExecutor;
class ParticleDrawExecutor;
class ParticleEmitterHolder;
class ParticleResourceHolder;

class EffectSystem : public NameObj {
public:
    EffectSystem(const char*, bool);

    ParticleEmitter* createEmitter(u16, u8, u8);
    void forceDeleteEmitter(ParticleEmitter*) const;
    void forceDeleteSingleEmitter(SingleEmitter*) const;

    void entry(ParticleResourceHolder*, u32, u32);

    u32 _C;
    ParticleEmitterHolder* mEmitterHolder;  // 0x10
    ParticleDrawExecutor* mDrawExec;        // 0x14
    ParticleCalcExecutor* mCalcExec;        // 0x18
    AutoEffectGroupHolder* mGroupHolder;    // 0x1C
};

namespace MR {
    EffectSystem* getEffectSystem();
};