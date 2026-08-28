#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class FixedPosition;
class HitSensor;
class MoguStone;
class ModelObj;

class Mogu : public LiveActor {
public:
    Mogu(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

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
