#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class Hanachan;

class HanachanParts : public LiveActor {
public:
    enum PartsType {
        PartsType_Head = 0,
        PartsType_Body = 1,
        PartsType_BodyS = 2,
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
    void exeAngryPursue();
    void exeAngryEnd();
    void exeWallHitEnd();
    void exeOverturn();
    void exeOverturnBound();
    void exeOverturnWait();
    void exeRecover();
    void exeHipDropped();
    void exeBlow();
    void exeStarPointerBind();
    bool isHeadHitWall();
    void changeHeadAngry();
    void changeHeadCalmDown();
    inline void endBecomeAngry();
    inline void endAngryPursue();
    inline void endRecover();
    inline void endStarPointerBind();
    bool isLandedInNerve(const Nerve*) const;
    bool isHipDroppedLanded() const;

    /* 0x8C */ Hanachan* mParent;
    /* 0x90 */ TVec3f mPushVelocity;
    /* 0x9C */ TVec3f mFallVelocity;
    /* 0xA8 */ PartsType mPartsType;
    /* 0xAC */ TQuat4f mRotation;
    /* 0xBC */ s32 mSegmentIndex;
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
    inline void endStarPointerBind();
    TVec3f calcSensorDirection(const HitSensor*, const HitSensor*) const;

    /* 0x8C */ HanachanParts* mBodyParts[5];
    /* 0xA0 */ TVec3f mAttackPos;
    /* 0xAC */ TVec3f mFrontDir;
    /* 0xB8 */ bool mIsChasePlayer;
    /* 0xBC */ AnimScaleController* mScaleController;
};
