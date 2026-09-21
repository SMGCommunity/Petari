#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;
class MapPartsRailMover;

class Banekiti : public LiveActor {
public:
    Banekiti(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initStarPointer();
    void exeWait();
    void exeRepel();
    void exeDPDSwoon();
    void endDPDSwoon();

    /* 0x8C */ AnimScaleController* mAnimScaleCtrl;
    /* 0x90 */ WalkerStateBindStarPointer* mBindStarPointer;
    /* 0x94 */ MapPartsRailMover* mRailMover;
};
