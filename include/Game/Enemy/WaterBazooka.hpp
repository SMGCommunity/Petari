#pragma once

#include "Game/Enemy/WaterBazookaCapsule.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class MogucchiShooter;
class ActorCameraInfo;
class JointController;
class CollisionParts;
class ModelObj;
class CameraTargetDemoActor;
class ElectricPressureBullet;

class WaterBazooka : public LiveActor {
public:
    WaterBazooka(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    bool isFirstShoot() const;
    bool isTired() const;
    bool isPanic() const;
    bool isBazookaPinch() const;
    bool isBazookaLifeOut() const;

    void exeWaitForBattle();
    void exeWait();
    void exeAim();
    void exeAimEnd();
    void exeShot();
    void exeShotNoMotion();
    void exeTire();
    void exeDemoCrackCapsule();
    void exeDemoAnger();
    void exeDemoBreakWait();
    void exeDemoBreakSign();
    void exeDemoBreakExplosion();
    void exeDemoBreakEnd();
    void exeWaitForLaugh();
    void exePanic();
    void exeStorm();
    void endStorm();

    void startDemoCrackCapsule();
    void startDemoAnger();
    void startDemoBreakCapsule();

    bool aimAtMario();
    void switchShowOrHide();
    void updateElectricLeak();
    void damageBazooka(HitSensor*, HitSensor*);
    bool calcJointCannon(TPos3f*, const JointControllerInfo&);
    s32 getSmokeLevel() const;  // enum?

    bool tryWaitForBattle();
    bool tryPanic();
    bool isNrvDemo() const;
    bool isElectricLeakSign() const;
    bool isElectricLeak() const;

    void initShooter();
    void initBazookaCapsule();
    void initBreakModel();
    void initCameraTarget();
    void initBullet(const JMapInfoIter&);
    bool tryShotBullet();
    ElectricPressureBullet* selectBulletElectric();
    bool tryJumpBackPlayerFromBazooka() const;
    void calcNearDropPoint(TVec3f*) const;
    void calcGunPointFromCannon(TPos3f*);
    void setCameraTargetMtx();

    inline void startBrk(const char* pName) {
        MR::startBrk(this, pName);
        MR::startBrk(mCapsule, pName);
    }

    /* 0x8C */ MogucchiShooter* mShooter;
    /* 0x90 */ WaterBazookaCapsule* mCapsule;
    /* 0x94 */ ModelObj* mBreakModel;
    /* 0x98 */ CameraTargetDemoActor* mDemoActor;
    /* 0x9C */ ActorCameraInfo* mCameraInfo;
    /* 0xA0 */ s32 mAlreadyDoneFlag;
    /* 0xA4 */ s32 mLife;
    /* 0xA8 */ s32 mShotNum;
    /* 0xAC */ CollisionParts* mCannonCollisionParts;
    /* 0xB0 */ JointController* mJointCtrl;
    /* 0xB4 */ TPos3f mBaseMtx;
    /* 0xE4 */ bool mIsPinchBGMStarted;
    /* 0xE5 */ bool mHasPowerStar;
    /* 0xE6 */ bool mIsElectric;
    /* 0xE8 */ ElectricPressureBullet** mBullets;
    /* 0xEC */ s32 mElectricTime;
};
