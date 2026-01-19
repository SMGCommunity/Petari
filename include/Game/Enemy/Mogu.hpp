#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HitSensor;
class AnimScaleController;
class MoguStone;
class FixedPosition;

class Mogu : public LiveActor {
public:
    Mogu(const char*);

    void init(const JMapInfoIter&);
    void kill();
    void control();
    void endClipped();
    void exeWait();
    void exeHide();
    void exeHideWait();
    void exeAppear();
    bool isPlayerExistUp();
    void exeSearch();
    void exeThrow();
    void tearDownThrow();
    void exeSwoonStart();
    void exeSwoon();
    void exeHipDropReaction();
    void exeSwoonEnd();
    void exeStampDeath();
    void exeHitBlow();
    bool isNearPlayerHipDrop();
    void attackSensor(HitSensor*, HitSensor*, bool);
    void receiveMsgPlayerAttack(u32, HitSensor*, bool);
    void tryPunchHitted(HitSensor*, HitSensor*, bool);
    void calcAndSetBaseMtx();

    /* 0x8C */ AnimScaleController* mNerveExecutor;
    FixedPosition* _90;
    /* 0x94 */ MoguStone* mStone;
    /* 0x98 */ ModelObj* mHole;
    TVec3f _9C;
    TVec3f _A8;
    bool _B4;
    /* 0xB5 */ bool mIsCannonFleet;
};
