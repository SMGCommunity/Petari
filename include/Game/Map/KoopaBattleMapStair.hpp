#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaBattleMapStair : public LiveActor {
public:
    enum Type {
        Type_Normal,
        Type_NoRequestFire,
        Type_DemoFar,
        Type_DemoNear,
    };

    KoopaBattleMapStair(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();

    bool isRequestAttackVs1() const;
    s32 calcRemainTimeToBreak() const;
    bool isRequestAttackVs3() const;
    f32 calcAndSetTargetPos(TVec3f*, const TVec3f&);
    f32 calcTimeRate() const;
    bool isBreak() const;
    bool isTypeNormal() const NO_INLINE;
    bool isTypeDemoFar() const;
    bool isTypeDemoNear() const;
    bool isTypeNoRequestFire() const NO_INLINE;

    void exeWaitSwitch();
    void exeWaitKoopaFire();
    void exeWaitFall();
    void exeFall();
    void exeDisappear();

    /* 0x8C */ s32 mTimeToBreak;  // Obj_arg0
    /* 0x90 */ s32 mFireAttackStep;
    /* 0x94 */ s32 mArg1;   // Obj_arg1
    /* 0x98 */ s32 mArg5;   // Obj_arg5
    /* 0x9C */ bool mArg6;  // Obj_arg6
    /* 0xA0 */ s32 mType;   // Obj_arg2
    /* 0xA4 */ bool mIsBig;
    /* 0xA5 */ bool mIsTurn;
    /* 0xA6 */ bool _A6;
    /* 0xA8 */ s32 _A8;
    /* 0xAC */ TVec3f _AC;
    /* 0xB8 */ s32 mWaitFallStep;  // Obj_arg3
    /* 0xBC */ s32 mFallStep;      // Obj_arg4
};
