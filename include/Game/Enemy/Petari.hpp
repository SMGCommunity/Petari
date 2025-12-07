#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;

class Petari : public LiveActor {
public:
    Petari(const char* pName);
    
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeApproach();
    void exeEscape();
    void exeSwoonStart();
    void exeSwoon();
    void exeSwoonEnd();
    void exeLand();
    void exeSmash();
    void exeSpinOut();
    void exeFreeze();
    void exeJumpOut();
    void exeLurk();
    void exeKickOut();
    
    void endFreeze();
    void endApproach();
    void endWait();
    void endEscape();
    
    void initFootPrint(const JMapInfoIter& rIter);
    void updateFootPrint();
    void initFrontVec();
    void initStarPiece(const JMapInfoIter& rIter);
    bool reflectStarPointer2P();
    bool tryShiftApproach();
    bool tryApproachEnd();
    bool tryShiftJumpOut();
    bool tryShiftEscape();
    void tryEmitFirstFootPrintEffect();
    void calcCenter();
    void calcApproachDirection();
    void calcEscapeDirection();
    void meander();
    void avoidPlayer();
    void avoidWall();
    f32 calcBoost(f32 baseSpeed, f32 boostMultiplier, f32 effectiveRange) const;
    void moveTowardTargetDirection(f32, f32, f32);
    bool tryEscapeEnd();
    void calcSpinOutVelocity(f32 speed);
    void appearStarPieceGradually();
    void appearStarPieceAll();
    bool isSolidBody() const;
    bool receivePlayerAttackAtBody(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    bool receivePlayerAttackAtSpin(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    void calcMouthPos(TVec3f* pOut) const;
    
    /* 8C */ AnimScaleController* mAnimScaleCtrl;
    /* 90 */ FootPrint* mFootPrint;
    /* 94 */ f32 mFootprintYOffs;
    /* 98 */ bool mRequestSmoke;
    /* 9C */ TVec3f mFront;
    /* A8 */ TVec3f mBodyCenter;
    /* B4 */ TVec3f mTargetDir;
    /* C0 */ s32 mMeanderStep;
    /* C4 */ f32 mMeanderAngle;
    /* C8 */ s32 mStarPieceTotal;
    /* CC */ s32 mStarPieceRemaining;
    /* D0 */ u8 _D0; // unused
    /* D4 */ TMtx34f mHeadMtx;
};
