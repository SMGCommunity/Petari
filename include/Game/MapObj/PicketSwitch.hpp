#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PicketSwitch : public LiveActor {
public:
    PicketSwitch(const char*);

    virtual ~PicketSwitch();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeFirstDrop();
    void exeLastDrop();
};
