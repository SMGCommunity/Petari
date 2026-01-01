#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CameraTargetMtx;
class ModelObj;

class Fluff : public LiveActor {
public:
    Fluff(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeFreeBloom();
    void exeFreeWaitOnGround();
    void exeFreeWaitAir();
    void exeFreeWaitInvalid();
    void exeRideStart();
    void exeRideFly();
    void exeRideFlyOnWind();
    void exeRideSpinUp();
    void exeRideSpinUpOnWind();

    bool updateRide();
    void updateWind();
    bool trySpinUp(s32 step);
    void endBind(f32 ejectForce);

    /* 0x8C */ TVec3f mSide;
    /* 0x98 */ TVec3f mUp;
    /* 0xA4 */ TVec3f mFront;
    /* 0xB0 */ TPos3f mInitialMtx;
    /* 0xE0 */ TVec3f mTargetUpVec;
    /* 0xEC */ TVec3f mWindDir;
    /* 0xF8 */ f32 mWindStrength;
    /* 0xFC */ s32 mWindSpinTimer;
    /* 0x100 */ s32 mSpinsRemaining;
    /* 0x104 */ ModelObj* mModel;
    /* 0x108 */ LiveActor* mRider;
    /* 0x10C */ CameraTargetMtx* mCameraTargetMtx;
};
