#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class RockCreator;

class Rock : public LiveActor {
public:
    Rock(f32 a1 = 10.0f, const char* pName = "ゴロゴロ岩");

    enum Type { NormalRock = 0, WanwanRolling = 1, WanwanRollingMini = 2, WanwanRollingGold = 3 };

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    static s32 getAppearFrame();
    static Type getType(const JMapInfoIter&);
    static s32 getAppearStarPieceNum(Type) NO_INLINE;
    void initMapToolInfo(const JMapInfoIter&);
    void initModel();
    void initSensor();
    void initEffect();
    bool isBreakByWall() const;
    bool isBodySensor(HitSensor*) const;  // stripped
    bool move(f32);
    void calcBaseMtx(TPos3f*) const;
    bool isInClippingRange() const;
    void startSoundWanwanVoice();
    void startRollLevelSound(bool);
    bool tryFreeze(const Nerve*);
    void setBtkForEnvironmentMap(LiveActor*, const char*);
    void rumblePadAndCamera();
    void setNerveBreak(bool) NO_INLINE;
    void updateRotateX(f32);
    void appearStarPiece();
    void moveOnRail(f32, f32, bool) NO_INLINE;
    bool isForceInvalidBindSection() const;
    bool tryBreakReachedGoal();
    const char* getTouchEffect() const;  // stripped
    bool isMoveEnabled() const;          // stripped

    void exeAppear();
    void exeAppearMoveInvalidBind();
    void exeMove();
    void exeMoveInvalidBind();
    void exeBreak();
    void exeFreeze();

    inline f32 getRadius() const {
        return mRockType == WanwanRollingMini ? 0.3f : mScale.x;
    }

    inline f32 getAngle() const {
        f32 rot = mRotation.x;
        return rot * PI_180;
    }

    inline bool isRockSensor(HitSensor* pSensor) const {
        return pSensor->isType(ATYPE_ROCK) || pSensor->isType(ATYPE_WANWAN);
    }

    /* 0x8C */ RockCreator* mCreator;
    /* 0x90 */ Type mRockType;
    /* 0x94 */ ModelObj* mBreakModel;
    /* 0x98 */ f32 mMoveSpeed;
    /* 0x9C */ bool mBreakModelOnRailGoal;
    /* 0x9D */ bool mSlowDownOnAttack;
    /* 0xA0 */ TVec3f mAppearPos;
    /* 0xAC */ f32 mRadius;
    /* 0xB0 */ f32 mRotateSpeed;
    /* 0xB4 */ TPos3f mBaseMtx;
    /* 0xE4 */ TVec3f mFallVelocity;
    /* 0xF0 */ f32 mGravityRate;
    /* 0xF4 */ s32 mCurrentRailPoint;
    /* 0xF8 */ TVec3f mPrevPos;
    /* 0x104 */ TVec3f mFront;
    /* 0x110 */ u32 mAirTime;
    /* 0x114 */ bool mIsNormalGravity;
    /* 0x118 */ s32 mWanwanVoiceTimer;
    /* 0x11C */ s32 mFreezeTime;
    /* 0x120 */ TVec3f mFreezePos;
    /* 0x12C */ const Nerve* mUnfreezeNerve;
    /* 0x130 */ s32 mInvalidBindTime;
    /* 0x134 */ s32 mAppearTime;
    /* 0x138 */ f32 mAppearAngle;
    /* 0x13C */ s32 mRollSoundTimer;
    /* 0x140 */ s32 mSlowDownTimer;
};
