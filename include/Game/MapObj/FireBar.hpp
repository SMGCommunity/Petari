#pragma once

#include "Game/LiveActor/ModelObj.h"

class FireBarBall : public ModelObj {
public:
    FireBarBall(LiveActor *);

    virtual ~FireBarBall();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    void controlEmitEffect();

    LiveActor* mFireBarParent;  // _90
};

class FireBar : public LiveActor {
public:
    FireBar(const char *);

    virtual ~FireBar();
    virtual void init(const JMapInfoIter &);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void updateHitSensor(HitSensor *);
    virtual void attackSensor(HitSensor *, HitSensor *);

    void exeWait();
    void initFireBarBall(const JMapInfoIter &);
    void fixFireBarBall();

    FireBarBall** mFireBalls;   // _8C
    s32 mFireBallCount;         // _90
    TVec3f _94;
    f32 mFireBarSpeed;          // _A0
    s32 mStickCount;            // _A4
    f32 mStickDistance;         // _A8
};

namespace NrvFireBar {
    NERVE(FireBarNrvWait);
};