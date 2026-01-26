#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class CollisionParts;

class SandUpDownTriRock : public MapObjActor {
public:
    SandUpDownTriRock(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeDown();
    void exeRest();
    void exeUp();
    void exeStop();

    void startDown();

    /* 0xC4 */ CollisionParts* mRockCollisionParts[3];
    /* 0xD0 */ s32 mTimer;
};
