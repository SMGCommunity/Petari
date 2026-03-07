#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapObjConnector.hpp"

class SpinLeverSwitch : public LiveActor {
public:
    SpinLeverSwitch(const char*);

    virtual ~SpinLeverSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeSwitchOn();
    void exeEnd();

    MapObjConnector* mConnector;  //_8C
};
