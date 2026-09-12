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

    virtual ~BombBird() override;
    virtual void init(const JMapInfoIter&) override;
    virtual void makeActorDead() override;
    virtual void startClipped() override;
    virtual void control() override;
    virtual void calcAndSetBaseMtx() override;
    virtual void attackSensor(HitSensor*, HitSensor*) override;
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*) override;
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*) override;

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

    /* 0x8C */ MR::FixedArray< BombBirdBomb*, 3 > mBombs;
    /* 0x98 */ BombBirdBomb* mHeldBomb;
    /* 0x9C */ TVec3f mFront;
    /* 0xA8 */ bool mSearchPlayer;
    /* 0xA9 */ bool mHoldBomb;
    /* 0xAC */ FixedPosition* mFixedPos;
    /* 0xB0 */ SpinHitController* mSpinHitController;
    /* 0xB4 */ AnimScaleController* mScaleController;
    /* 0xB8 */ WalkerStateBindStarPointer* mStarPointer;
};
