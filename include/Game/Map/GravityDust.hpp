#pragma once

#include "Game/Effect/MultiEmitterParticleCallBack.hpp"

class LiveActor;

class GravityDustParticleCallBack : public MultiEmitterParticleCallBack {
public:
    GravityDustParticleCallBack(const LiveActor* pActor);

    virtual ~GravityDustParticleCallBack();

    virtual void execute(JPABaseEmitter* pEmitter, JPABaseParticle* pParticle);

    /* 0x04 */ const LiveActor* mActor;
};
