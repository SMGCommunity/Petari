#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HitSensor;
class AnimScaleController;
class MoguStone;
class FixedPosition;

class Mogu : public LiveActor {
public:
    Mogu(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeHide();
    void exeHideWait();
    void exeAppear();
    bool isPlayerExistUp();
    void exeSearch();
    void exeThrow();
    void endThrow();
    void tearDownThrow();
    void exeSwoonStart();
    void exeSwoon();
    void exeHipDropReaction();
    void exeSwoonEnd();
    void exeStampDeath();
    void exeHitBlow();
    bool isNearPlayerHipDrop();
    bool tryPunchHitted(HitSensor*, HitSensor*, bool);

    /* 0x8C */ AnimScaleController* mAnimScaleController;
    FixedPosition* _90;
    /* 0x94 */ MoguStone* mStone;
    /* 0x98 */ ModelObj* mHole;
    /* 0x9C */ TVec3f mSight;
    TVec3f _A8;
    bool _B4;
    /* 0xB5 */ bool mIsCannonFleet;
};
