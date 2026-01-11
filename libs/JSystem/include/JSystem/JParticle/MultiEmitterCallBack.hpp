#pragma once

#include <JSystem/JParticle/JPAEmitterCallBack.hpp>

class MultiEmitterCallBackBase : public JPAEmitterCallBack {
public:
    virtual ~MultiEmitterCallBackBase();
    virtual void init(JPABaseEmitter*);
};
