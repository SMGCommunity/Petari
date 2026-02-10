#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;

class Snakehead : public LiveActor {
public:
    Snakehead(const char*);

    virtual ~Snakehead();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWaylay();
    void exeWait();
    void exeMoveForward();
    void exeRest();
    void exeMoveBack();
    void exeTurtleDown();
    void exeDPDSwoon();
    void endDPDSwoon();
    void choiceAndStartBck(const char*);
    bool tryDPDSwoon();
    bool isNearPlayerFromRail() const;

    AnimScaleController* mController;          // 0x8C
    WalkerStateBindStarPointer* mStarPointer;  // 0x90
    TPos3f _94;
    TVec3f _C4;
    f32 _D0;
    f32 _D4;
    f32 _D8;
    TVec3f _DC;
    s32 _E8;
    BenefitItemObj* mItem;  // 0xEC
};
