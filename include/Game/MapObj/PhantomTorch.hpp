#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinPullParticleCallBack;

class PhantomTorch : public LiveActor {
public:
    PhantomTorch(const char*);

    /* 0x08 */ virtual ~PhantomTorch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void soundOrder();
    void exeWait();
    void exeAttack();
    void exeExtinguish();

    const char* mParticle;
    s32 mItem;
    SpinPullParticleCallBack* mPullParticle;
    bool mIsDecorative;
};
