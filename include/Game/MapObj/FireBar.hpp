#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class FireBarBall : public ModelObj {
public:
    FireBarBall(LiveActor*);

    /* 0x08 */ virtual ~FireBarBall();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();

    void controlEmitEffect();

    LiveActor* mFireBarParent;  // 0x90
};

class FireBar : public LiveActor {
public:
    FireBar(const char*);

    /* 0x08 */ virtual ~FireBar();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

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
