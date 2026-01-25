#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinPullParticleCallBack;

class PhantomTorch : public LiveActor {
public:
    PhantomTorch(const char*);

    virtual ~PhantomTorch();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void soundOrder();
    void exeWait();
    void exeAttack();
    void exeExtinguish();

    const char* mParticle;
    s32 mItem;
    SpinPullParticleCallBack* mPullParticle;
    bool mIsDecorative;
};
