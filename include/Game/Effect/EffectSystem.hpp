#pragma once

#include "Game/NameObj/NameObj.hpp"

class AutoEffectGroupHolder;
class JPAEmitterManager;
class MultiEmitterCallBackBase;
class MultiEmitterParticleCallBack;
class ParticleCalcExecutor;
class ParticleDrawExecutor;
class ParticleEmitter;
class ParticleEmitterHolder;
class ParticleResourceHolder;
class SingleEmitter;

class EffectSystem : public NameObj {
public:
    EffectSystem(const char*, bool);

    virtual void init(const JMapInfoIter& rIter);

    ParticleEmitter* createEmitter(u16, u8, u8);
    void forceDeleteEmitter(ParticleEmitter*) const;
    void forceDeleteSingleEmitter(SingleEmitter*) const;
    void createSingleEmitter(SingleEmitter*, MultiEmitterCallBackBase*, MultiEmitterParticleCallBack*);
    void entry(ParticleResourceHolder*, u32, u32);

    /* 0x0C */ JPAEmitterManager* mEmitterManager;
    /* 0x10 */ ParticleEmitterHolder* mEmitterHolder;
    /* 0x14 */ ParticleDrawExecutor* mDrawExec;
    /* 0x18 */ ParticleCalcExecutor* mCalcExec;
    /* 0x1C */ AutoEffectGroupHolder* mGroupHolder;
    /* 0x20 */ bool _20;
};

namespace MR {
    EffectSystem* getEffectSystem();
};
