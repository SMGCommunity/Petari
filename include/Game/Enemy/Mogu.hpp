#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HitSensor;
class NerveExecutor;
class MoguStone;

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

    NerveExecutor* mNerveExecutor;
    u32 _90;
    MoguStone* mStone;
    NameObj* _98;
    TVec3f _9C;
    TVec3f _A8;
    bool _B4;
    bool mIsCannonFleet;
};
