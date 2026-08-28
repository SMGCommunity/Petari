#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PicketSwitch : public LiveActor {
public:
    PicketSwitch(const char*);

    /* 0x08 */ virtual ~PicketSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeFirstDrop();
    void exeLastDrop();
};
