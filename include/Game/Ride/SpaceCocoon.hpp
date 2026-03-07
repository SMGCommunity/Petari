#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PlantPoint;

class SpaceCocoon : public LiveActor {
public:
    SpaceCocoon(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeStop();
    void exeFree();
    void exeFreeInvalid();
    void exeBindLand();
    void exeBindWait();
    void exeWaitKinopioAimDemo();
    void exeBindAim();
    void exeBindAttack();
    void exeBindAttackSuccess();

    void calcAnim();
    bool updateBindWait();
    bool updateSpringPoint();
    void updateHang();
    void updateBindAttack();
    void updateActorMtx();
    void updateDrawPoints();

    bool tryTouch();
    bool tryRelease();
    bool tryAttackMap();
    bool isKinopioAttached() const;

    void endBind(const TVec3f&, bool);
    void endCommandStream();

    void drawPlane(f32, f32, f32, f32, Color8, Color8, f32, f32) const;
    void initDraw() const;

    /// @note if the next use of the SpaceCocoon should signal the associated camera area to stop focusing on this SpaceCocoon (for use when Kinipio
    /// is not yet launched from SpaceCocoon)
    /* 0x8C */ bool mIsKinopioCameraFocused;
    /* 0x90 */ TVec3f mNeutralPos;
    /* 0x9C */ TVec3f mBasePos;
    /* 0xA8 */ TVec3f mSide;
    /* 0xB4 */ TVec3f mUp;
    /* 0xC0 */ TVec3f mFront;
    /* 0xCC */ f32 mHeight;
    /* 0xD0 */ TVec3f mPointerPos;
    /* 0xDC */ s32 mPadChannel;
    /* 0xE0 */ TVec3f mCocoonPos;
    /* 0xEC */ TVec3f mSpringVel;

    /// @note objArg0: number of frames in "attack mode" in the air before regaining control
    /* 0xF8 */ s32 mAttackTime;
    /* 0xFC */ s32 mTouchTime;
    /* 0x100 */ LiveActor* mRider;

    /* 0x104 */ TPos3f mBaseMtx;
    /* 0x134 */ s32 mNumPoints;
    /* 0x138 */ PlantPoint** mPlantPoints;
    /* 0x13C */ CameraTargetMtx* mCameraTargetMtx;
    /* 0x140 */ ActorCameraInfo* mCameraInfo;
    /* 0x144 */ PartsModel* mCocoonModel;
    /* 0x148 */ TPos3f mTopMtx;
    /* 0x178 */ JUTTexture* mTexture;
};
