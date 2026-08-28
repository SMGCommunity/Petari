#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Hanachan;
class AnimScaleController;

class HanachanParts : public LiveActor {
public:
    HanachanParts(Hanachan*, s32, const char*, const char*);

    /* 0x08 */ virtual ~HanachanParts();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    const TVec3f* getCommonGravity() const;
    void exeWalk();
    void exeTrample();
    void exeBecomeAngry();
    void exeAngryPursue();
    void exeAngryEnd();
    void exeWallHitEnd();
    void exeOverturn();
    void exeOverturnBound();
    void exeOverturnWait();
    void exeOverturnHipDropped();
    void exeRecover();
    void exeHipDropped();
    void exeBlow();
    void exeStarPointerBind();
    bool isHeadHitWall();
    void changeHeadAngry();
    void changeHeadCalmDown();

    Hanachan* mParent;  // 0x8C
    TVec3f _90;
    TVec3f _9C;
    u32 _A8;
    Quaternion _AC;
    s32 _BC;
    s32 _C0;
    u8 _C4;
};

class Hanachan : public LiveActor {
public:
    Hanachan(const char*);

    /* 0x08 */ virtual ~Hanachan();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x2C */ virtual void kill();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();

    void exeWalk();
    void exeTrample();
    void exeBecomeAngry();
    void exeAngryPursue();
    void exeAngryEnd();
    void exeWallHitEnd();
    void exeOverturn();
    void exeOverturnWait();
    void exeOverturnBound();
    void exeRecover();
    void exeHipDropped();
    void exeBlow();
    void exeStarPointerBind();
    void exeStarPointerBindEnd();
    void exeStarPointerBindOverturn();
    s32 calcNearestInfectionId();
    bool isOwnSensor(HitSensor*);
    void setNerveBlow(const TVec3f&);
    void applyPlayerHipDropReaction();
    void setNerveAllParts(const Nerve*);
    void setDelayAllPartsAtId(s32, s32, s32);
    bool isStarPointerPointing();
    void moveHeadAlongRail(f32);
    void moveHeadToPlayer(f32, f32);
    void moveBodyAlongHead();

    HanachanParts* mBodyParts[5];  // 0x8C
    TVec3f _A0;
    TVec3f _AC;
    u8 _B8;
    AnimScaleController* mScaleCtrl;  // 0xBC
};
