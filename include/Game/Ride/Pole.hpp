#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Pole : public LiveActor {
public:
    Pole(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void calcAnim();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual MtxPtr getBaseMtx() const;

    void exeDemoAppear();
    void exeFree();
    void exeFreeInvalid();
    void exeBindStart();
    void exeBindWait();
    void exeBindTurn();
    void exeBindClimbUp();
    void exeBindFallDown();
    void exeBindHandstandStart();
    void exeBindHandstandWait();
    void exeBindHandstandEnd();
    void exeBindHandstandTurn();
    bool tryJump(bool, f32);
    bool tryTurn();
    bool tryClimbUp();
    bool tryFallDown();
    bool tryHandstandTurn();
    bool isEnableTurn() const;
    void updateBindTrans();
    void calcGravityMtx(TPos3f*);
    void updateTopPos(f32);
    f32 getPoleSubPadStickX() const;
    f32 getPoleSubPadStickY() const;

    /* 0x8C */ TVec3f mBasePos;
    /* 0x98 */ TVec3f mTopPos;
    /* 0xA4 */ f32 mPoleLength;
    /* 0xA8 */ bool mDisableHandstand;
    /* 0xA9 */ bool mIsSquare;
    /* 0xAA */ bool mNoModel;
    /* 0xAB */ bool mInvertStick;
    /* 0xAC */ bool mUsePole9mCollision;
    /* 0xB0 */ f32 mClimbCoord;
    /* 0xB4 */ f32 mClimbSpeed;
    /* 0xB8 */ LiveActor* mRider;
    /* 0xBC */ ActorCameraInfo* mCameraInfo;
    /* 0xC0 */ TVec3f mCenterPos;
    /* 0xCC */ TVec3f mSide;
    /* 0xD8 */ TVec3f mUp;
    /* 0xE4 */ TVec3f mFront;
    /* 0xF0 */ TPos3f mPosMtx;
    /* 0x120 */ MtxPtr mTopMtx;
    /* 0x124 */ MtxPtr mBottomMtx;
    /* 0x128 */ TMtx34f mBaseMtx;
};
