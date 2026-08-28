#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"

class AnimScaleController;
class BombBirdBomb;
class FixedPosition;
class SpinHitController;
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

    /* 0x08 */ virtual ~BombBird();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x30 */ virtual void makeActorDead();
    /* 0x40 */ virtual void startClipped();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual bool recieveOtherMsg(u32, HitSensor*, HitSensor*);

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
