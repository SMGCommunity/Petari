#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaBattleMapStair : public LiveActor {
public:
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

    s32 mFireTimer;  // _8C
    s32 _90;
    s32 _94;
    s32 _98;
    bool _9C;
    s32 mType;          // 0xA0
    bool mIsStairBig;   // 0xA4
    bool mIsStairTurn;  // 0xA5
    u8 _A6;
    u32 _A8;
    TVec3f _AC;
    s32 _B8;
    s32 _BC;
};
