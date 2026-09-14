#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry.hpp"

class AnimScaleController;
class AnimStampController;
class SpinHitController;
class WalkerStateBindStarPointer;
class JointController;
struct JointControllerInfo;

class KoteBug : public LiveActor {
public:
    KoteBug(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeNoCalcWait();
    void endNoCalcWait();
    void exeWait();
    void endWait();
    void exeSearch();
    void exeEscapeSearch();
    void exePreEscape();
    void exeFlyStart();
    void exeFlyPursue();
    void exeFlyPursueLast();
    void exeOverturnFall();
    void exeOverturn();
    void exePreRecover();
    void exeRecover();
    void exeHitToPlayer();
    void exeBlow();
    void exeStampDeath();
    void exeBindStarPointer();
    void endBindStarPointer();
    void setOverturnFall(HitSensor* pSender, HitSensor* pReceiver);
    bool calcJoint(TPos3f* pMtx, const JointControllerInfo& rInfo);

    /* 0x8C */ AnimScaleController* mScaleController;
    /* 0x90 */ AnimStampController* mStampController;
    /* 0x94 */ SpinHitController* mSpinHitController;
    /* 0x98 */ WalkerStateBindStarPointer* mStarPointer;
    /* 0x9C */ TVec3f mFront;
    /* 0xA8 */ TQuat4f mPosture;
    /* 0xB8 */ JointController* mJointController;
};
