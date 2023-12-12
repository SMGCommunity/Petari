#pragma once

#include "Game/LiveActor/LiveActor.h"

class BallRailPoint {
public:
    BallRailPoint();

    TVec3f _0;
    TVec3f _C;
    TVec3f _18;
    TVec3f _24;
};

class BallRail : public LiveActor {
public:
    BallRail(const char *);

    virtual ~BallRail();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initRailPoints();
    void exeWait();
    void exeSetUp();
    void exeRun();
    inline void exeNoBind();

    BallRailPoint* mRailPoints; // _8C
    HitSensor* _90;
    TVec3f _94;
    s32 mNumPoints;             // _A0
    f32 mAcceleration;          // _A4
    f32 mDeceleration;          // _A8
    f32 _AC;
};

namespace NrvBallRail {
    NERVE_DECL(BallRailNrvWait, BallRail, BallRail::exeWait);
    NERVE_DECL(BallRailNrvSetUp, BallRail, BallRail::exeSetUp);
    NERVE_DECL(BallRailNrvRun, BallRail, BallRail::exeRun);
    NERVE_DECL(BallRailNrvNoBind, BallRail, BallRail::exeNoBind);
};