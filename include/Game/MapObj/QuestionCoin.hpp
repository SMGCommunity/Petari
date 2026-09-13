#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;

class QuestionCoin : public LiveActor {
public:
    QuestionCoin(const char*);

    virtual ~QuestionCoin();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initFromJMapInfoArg(const JMapInfoIter&);
    void initShadow();
    void updateActorMtx();
    void exeWait() {
    }

    void exeThrowActor();
    void exeCaught();
    bool isThrowActor() const {
        return 0.0f < mLaunchSpeed;
    }

    /* 0x8C */ TPos3f mBaseMtx;
    /* 0xBC */ LiveActor* mBoundActor;
    /* 0xC0 */ TPos3f mActorMtx;
    /* 0xF0 */ TQuat4f mLaunchQuat;
    /* 0x100 */ ActorCameraInfo* mCameraInfo;
    /* 0x104 */ f32 mLaunchSpeed;
    /* 0x108 */ bool mIsWeakGravity;
    /* 0x109 */ bool mCalcShadow;
    /* 0x10C */ s32 mPoseType;
    /* 0x110 */ s32 mShadowDropLengthType;
    /* 0x114 */ TVec3f mShadowPos;
    /* 0x120 */ f32 mShadowDropLength;
};
