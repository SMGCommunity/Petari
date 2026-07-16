#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Hanachan;
class AnimScaleController;

class HanachanParts : public LiveActor {
public:
    HanachanParts(Hanachan*, s32, const char*, const char*);

    virtual ~HanachanParts();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

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
    TQuat4f _AC;
    s32 _BC;
    s32 _C0;
    u8 _C4;
};

class Hanachan : public LiveActor {
public:
    Hanachan(const char*);

    virtual ~Hanachan();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

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
