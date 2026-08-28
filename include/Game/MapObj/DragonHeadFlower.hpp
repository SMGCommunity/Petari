#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DragonHeadFlower : public LiveActor {
public:
    DragonHeadFlower(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initParabola(const TVec3f&);

    void exeWait();
    void exeSetCenter();
    void exeLaunchReady();
    void exeRailMove();

    /* 0x8C */ HitSensor* mPlayerSensor;
    /* 0x90 */ f32 mParabolaProgress;
    /* 0x94 */ TVec3f mParabolaStartPos;
    /* 0xA0 */ TVec3f mTargetVelocity;
    /* 0xAC */ TVec3f mCurrentGravity;
    /* 0xB8 */ TVec3f mEndGravity;
    /* 0xC4 */ TVec3f mParabolaDirection;
    /* 0xD0 */ u32 mParabolaSteps;
    /* 0xD4 */ f32 mParabolaDistance;
    /* 0xD8 */ f32 mParabolaAccel;
    /* 0xDC */ f32 mParabolaVel;
};
