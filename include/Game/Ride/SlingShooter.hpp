#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class CameraTargetMtx;

class SlingShooter : public LiveActor {
public:
    SlingShooter(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x1C */ virtual void calcAnim();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x50 */ virtual void updateHitSensor(HitSensor* pSensor);
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

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

    /// @note if the next use of the SlingShooter should signal the associated camera area to stop focusing on this SlingShooter (for use when Kinipio
    /// is not yet launched from SlingShooter)
    /* 0x8C */ bool mIsKinopioCameraFocused;  // maybe rename like SpaceCocoon?
    /* 0x90 */ u32 _90;                       // unused
    /* 0x94 */ f32 mPlayerZ;
    /* 0x98 */ bool mIsKinopioAttached;
    /* 0x99 */ bool mIsKinopioTalkEnabled;
    /* 0x9A */ bool mIsLastDamage;
    /* 0x9C */ const TVec3f* mBasePos;
    /* 0xA0 */ const TVec3f* mNeutralPos;
    /* 0xA4 */ const TVec3f* mUp;
    /* 0xA8 */ TVec3f mPointerPos;
    /* 0xB4 */ s32 mPadChannel;
    /* 0xB8 */ bool mPassThroughEnable;
    /* 0xBC */ TVec3f mAttackVelocity;
    /* 0xC8 */ LiveActor* mRider;
    /* 0xCC */ TPos3f mBaseMtx;
    /* 0xFC */ CameraTargetMtx* mCameraTargetMtx;
    /* 0x100 */ ActorCameraInfo* mCameraInfo;
};
