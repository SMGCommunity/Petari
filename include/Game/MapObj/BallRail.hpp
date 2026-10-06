#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BallRailPoint {
public:
    BallRailPoint();

    /* 0x00 */ TVec3f _0;
    /* 0x0C */ TVec3f _C;
    /* 0x18 */ TVec3f _18;
    /* 0x24 */ TVec3f _24;
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

    /* 0x8C */ BallRailPoint* mRailPoints;
    /* 0x90 */ HitSensor* _90;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ s32 mNumPoints;
    /* 0xA4 */ f32 mAcceleration;
    /* 0xA8 */ f32 mDeceleration;
    /* 0xAC */ f32 _AC;
};
