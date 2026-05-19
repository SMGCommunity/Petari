#pragma once

#include "Game/Effect/MultiEmitterParticleCallBack.hpp"

class LiveActor;

class GravityDustParticleCallBack : public MultiEmitterParticleCallBack {
public:
    GravityDustParticleCallBack(const LiveActor*);

    virtual void execute(JPABaseEmitter*, JPABaseParticle*);

    LiveActor* mActor;
};
