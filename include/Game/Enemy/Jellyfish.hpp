#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;

class Jellyfish : public LiveActor {
public:
    Jellyfish(const char*);

    /* 0x08 */ virtual ~Jellyfish();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeFind();
    void exeThreat();
    void exeDeath();
    void exeAttack();
    void exeRailGoal();
    void exeDPDSwoon();
    void exeWaitWithLeftTurn();
    void exeWaitWithRightTurn();
    void exeThreatWithLeftTurn();
    void exeThreatWithRightTurn();
    void threatTurn();
    bool faceToMario();
    void knockOut(HitSensor*, HitSensor*);
    bool selectNerveAfterWait();
    bool selectNerveThreat();
    bool tryDPDSwoon();

    AnimScaleController* mController;          // 0x8C
    WalkerStateBindStarPointer* mBindStarPtr;  // 0x90
    s32 _94;
    TVec3f _98;
    bool mIsConnectedRail;  // 0xA4
    f32 _A8;
    s32 _AC;
    bool _B0;
    TVec3f _B4;
};
