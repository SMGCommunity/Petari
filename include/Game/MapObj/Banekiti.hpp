#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class MapPartsRailMover;
class WalkerStateBindStarPointer;

class Banekiti : public LiveActor {
public:
    Banekiti(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeRepel();
    void exeDPDSwoon();
    void endDPDSwoon();

    AnimScaleController* mAnimScaleCtrl;           //_8C
    WalkerStateBindStarPointer* mBindStarPointer;  //_90
    MapPartsRailMover* mRailMover;                 //_94
};
