#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class TeresaWater : public LiveActor {
public:
    TeresaWater(const char*);
    virtual ~TeresaWater();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeMove();
    void exeHit();
    void exeStun();
    bool tryDPDSwoon();
    void exeDPDSwoon();
    void endDPDSwoon();

private:
    AnimScaleController* mScaleController;
    WalkerStateBindStarPointer* mBindStarPointer;
    TVec3f _90;
    TVec3f _A0;
    TVec3f _AC;
    TVec3f* _B8;
};
