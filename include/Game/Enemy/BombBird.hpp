#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FixedPosition;
class SpinHitController;
class BombBirdBomb;
class AnimScaleController;
class WalkerStateBindStarPointer;

struct BombBirdParam {
    f32 _0;
    f32 _4;
    f32 _8;
    f32 _C;
};

class BombBird : public LiveActor {
public:
    BombBird(const char*);

    virtual ~BombBird();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorDead();
    virtual void startClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool recieveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeFlyOnRail();
    void exeFlyOnRailSearch();
    void doFly();
    void addVelocityToUpFromGround(f32);
    void killAllBomb();
    void exeFlyWithAttack();
    void exeBlow();
    void exeEscape();
    void exeDamage();
    void exeFall();
    void exeSwoonStart();
    void exeSwoon();
    void exeSwoonEnd();
    void exeTrample();
    void exeSinkDown();
    void exeBindStarPointer();
    void endBindStarPointer();

    MR::FixedArray< BombBirdBomb*, 4 > mBombs;  // 0x8C
    // BombBirdBomb* mBombs[4];  // 0x8C
    TVec3f _9C;
    u8 _A8;
    s8 _A9;
    FixedPosition* mFixedPos;                  // 0xAC
    SpinHitController* mSpinHitController;     // 0xB0
    AnimScaleController* mScaleController;     // 0xB4
    WalkerStateBindStarPointer* mStarPointer;  // 0xB8
};
