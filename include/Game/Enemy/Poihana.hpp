#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class AnimScaleController;
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
    void updateBindActorMtx();
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

    AnimScaleController* mAnimScaleCtrl;           // 0x8C
    WalkerStateBindStarPointer* mBindStarPointer;  // 0x90
    ActorCameraInfo* mCamInfo;                     // 0x94
    LiveActor* mBindedActor;                       // 0x98
    f32 mLaunchIntensity;                          // 0x9C
    TVec3f mRespawnPos;                            // 0xA0
    f32 _AC;                                       // 0xAC
    f32 _B0;                                       // 0xB0
    TVec3f mHomePos;                               // 0xB4
    f32 _C0;                                       // 0xC0
    s32 mBoundTimer;                               // 0xC4
    s32 mRandDir;                                  // 0xC8
    s32 mBehavior;                                 // 0xCC
    bool mCanDrown;                                // 0xD0
    ModelObj* mWaterColumn;                        // 0xD4
    TVec3f mFrontVec;                              // 0xD8
    bool mIsActive;                                // 0xE4
    u8 _E5;                                        // 0xE5
};
