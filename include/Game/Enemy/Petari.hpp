#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class FootPrint;

class Petari : public LiveActor {
public:
    Petari(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x1C */ virtual void calcAnim();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeLurk();
    void exeJumpOut();
    void exeWait();
    void exeApproach();
    void exeEscape();
    void exeSwoonStart();
    void exeSwoon();
    void exeSwoonEnd();
    void exeLand();
    void exeSmash();
    void exeSpinOut();
    void exeKickOut();
    void exeFreeze();

    void endApproach();
    void endWait();
    void endFreeze();
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

    /* 0x8C */ AnimScaleController* mAnimScaleCtrl;
    /* 0x90 */ FootPrint* mFootPrint;
    /* 0x94 */ f32 mFootprintYOffs;
    /* 0x98 */ bool mRequestSmoke;
    /* 0x9C */ TVec3f mFront;
    /* 0xA8 */ TVec3f mBodyCenter;
    /* 0xB4 */ TVec3f mTargetDir;
    /* 0xC0 */ s32 mMeanderStep;
    /* 0xC4 */ f32 mMeanderAngle;
    /* 0xC8 */ s32 mStarPieceTotal;
    /* 0xCC */ s32 mStarPieceRemaining;
    /* 0xD0 */ u8 _D0;  // unused
    /* 0xD4 */ TMtx34f mHeadMtx;
};
