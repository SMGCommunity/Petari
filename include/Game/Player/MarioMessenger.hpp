#pragma once

#include "Game/NameObj/NameObj.hpp"

class HitSensor;

class MarioMessenger : NameObj {
public:
    MarioMessenger(HitSensor* pSender);

    virtual ~MarioMessenger();
    virtual void movement();

    void addRequest(HitSensor* pReceiver, u32 msg);

public:
    /* 0x0C */ HitSensor* mSender;
    /* 0x10 */ HitSensor** mReceiverArray;
    /* 0x14 */ u32* mMsgArray;
    /* 0x18 */ u32 mArraySize;
};
