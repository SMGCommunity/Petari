#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Butterfly : public LiveActor {
public:
    Butterfly(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void updatePosture();
    void addRunAwayVelocity();
    bool tryRunAway();
    bool tryHive();
    bool tryPerchOnSleepingMario();
    bool tryAppearStarPeace(s32 port);

    void exeWait();
    void exeRunAway();
    void exeHive();
    void exePerchOn();
    void exeGotoSleepingMario();
    void exeReadyToPerchOnSleepingMario();
    void exePerchOnSleepingMario();

private:
    /* 0x8C */ TVec3f mHomePos;
    /* 0x98 */ TQuat4f mQuat;
    /* 0xA8 */ s32 mColorIndex;
    /* 0xAC */ HitSensor* mPerchTarget;
    /* 0xB0 */ bool mStarPieceAppeared;
};
