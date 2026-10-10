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

    /* 0x90 */ LiveActor* mFireBarParent;
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

    /* 0x8C */ FireBarBall** mFireBalls;
    /* 0x90 */ s32 mFireBallCount;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ f32 mFireBarSpeed;
    /* 0xA4 */ s32 mStickCount;
    /* 0xA8 */ f32 mStickDistance;
};
