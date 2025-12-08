#pragma once

#include <JSystem/JParticle/JPAEmitterCallBack.hpp>

class JPABaseParticle;

class JPAParticleCallBack {
public:
    virtual ~JPAParticleCallBack();
    virtual void execute(JPABaseEmitter*, JPABaseParticle*) {}

    virtual void draw(JPABaseEmitter*, JPABaseParticle*) {}
};
