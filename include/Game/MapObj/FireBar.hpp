#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class FireBarBall : public ModelObj {
public:
    FireBarBall(LiveActor*);

    virtual ~FireBarBall();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    void controlEmitEffect();

    LiveActor* mFireBarParent;  // 0x90
};

class FireBar : public LiveActor {
public:
    FireBar(const char*);

    virtual ~FireBar();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void initFireBarBall(const JMapInfoIter&);
    void fixFireBarBall();

    FireBarBall** mFireBalls;  // 0x8C
    s32 mFireBallCount;        // 0x90
    TVec3f _94;
    f32 mFireBarSpeed;   // 0xA0
    s32 mStickCount;     // 0xA4
    f32 mStickDistance;  // 0xA8
};
