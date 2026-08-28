#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class JointController;
class JointControllerInfo;
class WalkerStateBindStarPointer;

class BombHei : public LiveActor {
public:
    BombHei(const char*);

    enum Type { BOMB_HEI = 0, BOMB = 1 };

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x6C */ virtual bool receiveMsgThrow(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void setGroupClipping(const JMapInfoIter& rIter);
    void startCountdown();
    void endCountdown();

    void exeLaunch();
    void exeStarting();
    void exeWait();
    void exeWalk();
    void exePursue();
    void exePursueHit();
    void exeSpinHit();
    void endSpinHit();
    void exeTrample();
    void exePhysics();
    void exeStop();
    void exeTaken();
    void endTaken();
    void exeThrown();
    void endThrown();
    void exeExplode();
    void exeBindStarPointer();
    void endBindStarPointer();

    bool calcJoint(TPos3f*, const JointControllerInfo&);
    void startBoundSound();

    /* 0x8C */ AnimScaleController* mScaleController;
    /* 0x90 */ WalkerStateBindStarPointer* mBindStarPointerState;
    /* 0x94 */ TQuat4f mRotQuat;
    /* 0xA4 */ TVec3f mFront;
    /* 0xB0 */ TVec3f mBinderOffset;
    /* 0xBC */ TVec3f mTargetFront;
    /* 0xC8 */ s32 mCountdown;
    /* 0xCC */ HitSensor* mCarrySensor;
    /* 0xD0 */ f32 mExplodeRadius;
    /* 0xD4 */ JointController* mBodyJoint;
    /* 0xD8 */ Type mType;
    /* 0xDC */ bool mExplosionIgnoreCollision;
};

struct BombHeiParam {
    /* 0x00 */ f32 mSpeedH;
    /* 0x04 */ f32 mGravAccel;
    /* 0x08 */ f32 mFriction;
    /* 0x0C */ f32 mTurnRate;
};
