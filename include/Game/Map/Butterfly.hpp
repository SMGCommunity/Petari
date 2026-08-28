#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Butterfly : public LiveActor {
public:
    Butterfly(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

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
