#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class MapPartsRailMover;
class WalkerStateBindStarPointer;

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
