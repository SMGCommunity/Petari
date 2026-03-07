#pragma once

#include "Game/LiveActor/LiveActor.hpp"

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
    BallRail(const char*);

    virtual ~BallRail();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initRailPoints();
    void exeWait();
    void exeSetUp();
    void exeRun();
    inline void exeNoBind();

    BallRailPoint* mRailPoints;  // 0x8C
    HitSensor* _90;
    TVec3f _94;
    s32 mNumPoints;     // 0xA0
    f32 mAcceleration;  // 0xA4
    f32 mDeceleration;  // 0xA8
    f32 _AC;
};
