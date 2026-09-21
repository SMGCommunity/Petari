#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class AnimScaleController;
class ModelObj;
class WalkerStateBindStarPointer;

class Poihana : public LiveActor {
public:
    Poihana(const char* pName);

    virtual ~Poihana();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeNonActive();
    void endNonActive();
    void exeWait();
    void exeWalkAround();
    void exeSleepStart();
    void exeSleep();
    void exeGetUp();
    void exeSearch();
    void exeChasePlayer();
    void exeShootUpCharge();
    void exeShootUp();
    void endShootUp() NO_INLINE;
    void exeGoBack();
    void exeShock();
    void exeSwoonLand();
    void exeSwoon();
    void exeRecover();
    void exeShake();
    void exeDrown();
    void exeHide();
    void exeAppear();
    void exeDPDSwoon();
    void endDPDSwoon() NO_INLINE;

    bool tryToStartBind(HitSensor* pSender);
    void updateBindActorMtx() NO_INLINE;
    void endBind();
    void startBound();
    void calcBound();
    void contactMario(HitSensor* pSender, HitSensor* pReceiver);
    void controlVelocity();
    void calcMyGravity();
    bool tryNonActive();
    bool tryDrown();
    bool tryDPDSwoon();
    bool tryShock();
    bool tryHipDropShock();
    bool isNerveTypeWalkOrWait() const;
    bool isNeedForBackHome() const;
    bool isNeedForGetUp() const;
    bool isBackAttack(HitSensor* pMySensor) const NO_INLINE;

    /* 0x8C */ AnimScaleController* mAnimScaleCtrl;
    /* 0x90 */ WalkerStateBindStarPointer* mBindStarPointer;
    /* 0x94 */ ActorCameraInfo* mCamInfo;
    /* 0x98 */ LiveActor* mBindedActor;
    /* 0x9C */ f32 mLaunchIntensity;
    /* 0xA0 */ TVec3f mRespawnPos;
    /* 0xAC */ TVec3f mHomePos;
    /* 0xB8 */ TVec3f mFrontVec;
    /* 0xC4 */ s32 mBoundTimer;
    /* 0xC8 */ s32 mRandDir;
    /* 0xCC */ s32 mBehavior;
    /* 0xD0 */ bool mCanDrown;
    /* 0xD4 */ ModelObj* mWaterColumn;
    /* 0xD8 */ TVec3f _D8;
    /* 0xE4 */ bool mIsActive;
    /* 0xE5 */ u8 _E5;
};
