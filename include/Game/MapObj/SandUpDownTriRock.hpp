#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class CollisionParts;

class SandUpDownTriRock : public MapObjActor {
public:
    /// @brief Creates a new `SandUpDownTriRock`.
    /// @param pName A pointer to the null-terminated name of the object.
    SandUpDownTriRock(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);

    void startDown();

    void exeWait();
    void exeDown();
    void exeRest();
    void exeUp();
    void exeStop();

    /* 0xC4 */ CollisionParts* mRockCollisionParts[3];
    /* 0xD0 */ s32 mRestStep;
};
