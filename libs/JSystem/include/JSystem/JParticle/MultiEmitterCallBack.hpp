#pragma once

#include <JSystem/JParticle/JPAEmitterCallBack.hpp>

class MultiEmitterCallBackBase : public JPAEmitterCallBack {
    virtual ~MultiEmitterCallBackBase();
    virtual void init(JPABaseEmitter*);
};
