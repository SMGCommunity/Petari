#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"

class AnimScaleController;
class Hanachan;

class HanachanParts : public LiveActor {
public:
    enum PartsType {
        /* 0x0 */ PartsType_Head,
        /* 0x1 */ PartsType_Body,
        /* 0x2 */ PartsType_BodyS,
    };

    HanachanParts(Hanachan*, s32, const char*, const char*);

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
    void endBecomeAngry();
    void exeAngryPursue();
    void endAngryPursue();
    void exeAngryEnd();
    void exeWallHitEnd();
    void exeOverturn();
    void exeOverturnBound();
    void exeOverturnWait();
    void exeRecover();
    void endRecover();
    void exeHipDropped();
    void exeBlow();
    void exeStarPointerBind();
    void endStarPointerBind();
    bool isHeadHitWall();
    bool isLandedInNerve(const Nerve*);
    bool isHipDroppedLanded();
    void changeHeadAngry();
    void changeHeadCalmDown();

    /* 0x8C */ Hanachan* mHost;
    /* 0x90 */ TVec3f mPushVelocity;
    /* 0x9C */ TVec3f mFallVelocity;
    /* 0xA8 */ PartsType mPartsType;
    /* 0xAC */ TQuat4f mRotationQuat;
    /* 0xBC */ s32 mPartsIndex;
    /* 0xC0 */ s32 mActionStartStep;
    /* 0xC4 */ bool mIsLanded;
};

class Hanachan : public LiveActor {
public:
    Hanachan(const char*);

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
    void endStarPointerBind();
    s32 calcNearestInfectionId();
    bool isOwnSensor(HitSensor*);
    void setNerveBlow(const TVec3f&);
    void applyPlayerHipDropReaction();
    void setNerveAllParts(const Nerve*);
    void setDelayAllPartsAtId(s32, s32, s32);
    bool isStarPointerPointing();
    void moveHeadAlongRail(f32);
    void moveHeadToPlayer(f32, f32);
    TVec3f calcSensorDirection(const HitSensor*, const HitSensor*) const;
    void moveBodyAlongHead();

    /* 0x8C */ MR::FixedArray< HanachanParts*, 5 > mBodyParts;
    /* 0xA0 */ TVec3f mAttackPos;
    /* 0xAC */ TVec3f mFrontDir;
    /* 0xB8 */ bool mIsChasePlayer;
    /* 0xBC */ AnimScaleController* mScaleController;
};
