#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class BenefitItemObj;
class WalkerStateBindStarPointer;

class Snakehead : public LiveActor {
public:
    Snakehead(const char*);

    /* 0x08 */ virtual ~Snakehead();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

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
