#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SwingRopePoint;

class JumpBranch : public LiveActor {
public:
    JumpBranch(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x50 */ virtual void updateHitSensor(HitSensor* pSensor);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeWaitInvalid();
    void exeBind();

    bool updateBind();

    /* 0x8C */ TVec3f mCenter;
    /* 0x98 */ TVec3f mSide;
    /* 0xA4 */ TVec3f mUp;
    /* 0xB0 */ TVec3f mFront;
    /* 0xBC */ LiveActor* mRider;
    /* 0xC0 */ SwingRopePoint* mSwingPoint;
    /* 0xC4 */ bool mSwingReverse;
};
