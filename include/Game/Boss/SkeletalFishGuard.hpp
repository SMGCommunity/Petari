#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class SkeletalFishBoss;

class SkeletalFishGuard : public LiveActor {
public:
    SkeletalFishGuard(SkeletalFishBoss*, const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeAppear();
    void exeNormal();
    void exeApart();
    void exeFollow();
    void exeStraight();
    void exeDefence();
    void exeKill();
    void exeNumb();
    void endNumb();

    void appearNaturally();
    void appearForce();
    void killNaturally();
    void waitAttack(s32);
    void initSensor();
    void calcTransAndFront();
    void rotateHorizontal(const TVec3f&, f32);
    void rotateVertical(const TVec3f&, f32);
    bool tryShiftApart();
    bool tryShiftStraight();
    bool tryShiftKill();
    void turn(TVec3f*, const TVec3f&, const TVec3f&, f32);
    void lookToPlayer(f32, f32);
    void calcTarget(TVec3f*, TVec3f*, TVec3f*, s32);
    bool isInScreen() const NO_INLINE;
    bool isPlayerInAttackRange() const;
    bool isLineOfSightClear() const;
    bool tryShiftNumb(const Nerve*);

    /* 0x08C */ u32 _8C;
    /* 0x090 */ SkeletalFishBoss* mFishBoss;
    /* 0x094 */ TVec3f _94;
    /* 0x0A0 */ u32 _A0;
    /* 0x0A4 */ TVec3f _A4;
    /* 0x0B0 */ f32 _B0;
    /* 0x0B4 */ f32 _B4;
    /* 0x0B8 */ f32 _B8;
    /* 0x0BC */ f32 _BC;
    /* 0x0C0 */ s32 mAttackDelay;
    /* 0x0C4 */ f32 _C4;
    /* 0x0C8 */ f32 _C8;
    /* 0x0CC */ const Nerve* _CC;
    /* 0x0D0 */ TVec3f _D0;
    /* 0x0DC */ TVec3f _DC;
    /* 0x0E8 */ TVec3f _E8;
    /* 0x0F4 */ TVec3f _F4;
    /* 0x100 */ TVec3f _100;
    /* 0x10C */ TVec3f _10C;
    /* 0x118 */ AnimScaleController* mScaleController;
};
