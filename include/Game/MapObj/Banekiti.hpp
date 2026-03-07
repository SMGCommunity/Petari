#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"

class Banekiti : public LiveActor {
public:
    Banekiti(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeRepel();
    void exeDPDSwoon();
    void endDPDSwoon();

    AnimScaleController* mAnimScaleCtrl;           //_8C
    WalkerStateBindStarPointer* mBindStarPointer;  //_90
    MapPartsRailMover* mRailMover;                 //_94
};
