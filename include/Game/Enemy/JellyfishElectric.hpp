#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;

class JellyfishElectric : public LiveActor {
public:
    JellyfishElectric(const char*);

    virtual ~JellyfishElectric();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeWaitWithLeftTurn();
    void exeWaitWithRightTurn();
    void exeDamage();
    void exeDeath();
    void exeAttack();
    void exeRailGoal();
    void exeDPDSwoon();
    void waitTurn();
    void knockOut();
    bool tryToAttackElectric(HitSensor*, HitSensor*);
    bool tryDPDSwoon();
    bool selectNerveAfterWait();

    AnimScaleController* mController;          // 0x8C
    WalkerStateBindStarPointer* mBindStarPtr;  // 0x90
    s32 _94;
    TVec3f _98;
    bool _A4;  // 0xA4
    bool mIsConnectedRail;
    f32 _A8;
    s32 _AC;
    bool _B0;
    TVec3f _B4;
};
