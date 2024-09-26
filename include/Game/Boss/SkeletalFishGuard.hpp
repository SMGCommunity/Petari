#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class SkeletalFishBoss;

class SkeletalFishGuard : public LiveActor {
public:
    SkeletalFishGuard(SkeletalFishBoss *, const char *);

    virtual ~SkeletalFishGuard();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void exeAppear();
    void exeNormal();
    void exeApart();
    void exeFollow();
    void exeStraight();
    void exeKill();
    void exeNumb();

    inline void exeOnEndNumb() {
        MR::deleteEffect(this, "PointerTouchManual");
        mScaleController->startAnim();
    }

    inline void exeWait();

    void appearNaturally();
    void appearForce();
    void killNaturally();
    void waitAttack(s32);
    void initSensor();
    void calcTransAndFront();
    void rotateHorizontal(const TVec3f &, f32);
    void rotateVertical(const TVec3f &, f32);
    bool tryShiftApart();
    bool tryShiftStraight();
    bool tryShiftKill();
    void turn(TVec3f *, const TVec3f &, const TVec3f &, f32);
    void lookToPlayer(f32, f32);
    void calcTarget(TVec3f *, TVec3f *, TVec3f *, s32);
    bool isInScreen() const;
    bool isPlayerInAttackRange() const;
    bool isLineOfSightClear() const;
    bool tryShiftNumb(const Nerve *);

    u32 _8C;
    SkeletalFishBoss* mFishBoss;                    // 0x90
    TVec3f _94;
    u32 _A0;
    TVec3f _A4;
    f32 _B0;
    f32 _B4;
    f32 _B8;
    f32 _BC;
    s32 mAttackDelay;                               // 0xC0
    f32 _C4;
    f32 _C8;
    const Nerve* _CC;
    TVec3f _D0;
    TVec3f _DC;
    TVec3f _E8;
    TVec3f _F4;
    TVec3f _100;
    TVec3f _10C;
    AnimScaleController* mScaleController;          // 0x118
};

namespace {
    NERVE_DECL(SkeletalFishGuardNrvWait, SkeletalFishGuard, SkeletalFishGuard::exeWait);
    NERVE_DECL(SkeletalFishGuardNrvAppear, SkeletalFishGuard, SkeletalFishGuard::exeAppear);
    NERVE_DECL(SkeletalFishGuardNrvNormal, SkeletalFishGuard, SkeletalFishGuard::exeNormal);
    NERVE_DECL(SkeletalFishGuardNrvApart, SkeletalFishGuard, SkeletalFishGuard::exeApart);
    NERVE_DECL(SkeletalFishGuardNrvFollow, SkeletalFishGuard, SkeletalFishGuard::exeFollow);
    NERVE_DECL(SkeletalFishGuardNrvStraight, SkeletalFishGuard, SkeletalFishGuard::exeStraight);
    NERVE_DECL_NULL(SkeletalFishGuardNrvDefence);
    NERVE_DECL(SkeletalFishGuardNrvKill, SkeletalFishGuard, SkeletalFishGuard::exeKill);
    NERVE_DECL_ONEND(SkeletalFishGuardNrvNumb, SkeletalFishGuard, SkeletalFishGuard::exeNumb, SkeletalFishGuard::exeOnEndNumb);
};