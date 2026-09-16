#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class CameraTargetMtx;

class SlingShooter : public LiveActor {
public:
    SlingShooter(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeFree();
    void exeFreeInvalid();
    void exeLand();
    void exeWait();
    void exeAim();
    void exeAttack();
    void exeAttackSuccess();

    void calcBaseMtx(TPos3f*);

    bool updateWait();
    void updateHang();
    void updateActorMtx();

    bool tryCancel();
    bool tryRelease();
    bool tryAttackMap();

    void startBind(LiveActor*);
    void startCancelWait();
    void startCancelAim();

    void endBind();
    void endCommandStream();

    void emitNullEffect(HitSensor* pSender, HitSensor* pReceiver);
    void emitHitEffect(HitSensor* pSender, HitSensor* pReceiver);

    /* 0x08C */ bool mIsKinopioCameraFocused;
    /* 0x090 */ u32 _90;
    /* 0x094 */ f32 mPlayerZ;
    /* 0x098 */ bool mIsKinopioAttached;
    /* 0x099 */ bool mIsKinopioTalkEnabled;
    /* 0x09A */ bool mIsLastDamage;
    /* 0x09C */ const TVec3f* mBasePos;
    /* 0x0A0 */ const TVec3f* mNeutralPos;
    /* 0x0A4 */ const TVec3f* mUp;
    /* 0x0A8 */ TVec3f mPointerPos;
    /* 0x0B4 */ s32 mPadChannel;
    /* 0x0B8 */ bool mPassThroughEnable;
    /* 0x0BC */ TVec3f mAttackVelocity;
    /* 0x0C8 */ LiveActor* mRider;
    /* 0x0CC */ TPos3f mBaseMtx;
    /* 0x0FC */ CameraTargetMtx* mCameraTargetMtx;
    /* 0x100 */ ActorCameraInfo* mCameraInfo;
};
