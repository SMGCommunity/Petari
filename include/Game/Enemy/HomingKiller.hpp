#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HomingKiller : public LiveActor {
public:
    enum Type { Type_HomingKiller = 0, Type_Torpedo = 1, Type_MagnumKiller = 2 };

    HomingKiller(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void appear(const TVec3f&, const TVec3f&);
    bool isMoveStart() const;
    void setChaseStartEndDistance(f32, f32);
    void initMapToolInfo(const JMapInfoIter&);
    void calcInitPosture();
    void updateVelocity();
    void updateRotateZ(const TVec3f&);
    bool processMove();
    bool processChase();

    bool isChaseStart() const;
    bool tryChaseStart();
    bool tryFreeze(const Nerve*);
    bool tryBindedBreak();
    bool tryToExplosion(HitSensor* pSender, HitSensor* pReceiver);
    void sendMsgExplosionToNearActor();

    bool isUpdateChaseFrontVec(const TVec3f&) const;
    void calcFrontVecToTarget(TVec3f*) const;

    bool isValidShowModel() const;
    bool isWaterBreak() const;

    void updateBaseMtxNoRotateZ();
    void startMoveLevelSound(bool);
    void setBckRate(f32, bool);
    bool isChasing() const;

    void exeAppear();
    void exeMoveStart();
    void exeMove();
    void exeChaseStart();
    void exeChase();
    void exeFreeze();
    void exeBreak();
    void exeGoToTarget();

    inline f32 getScale() {
        return mType == Type_MagnumKiller ? 4.0f : 1.0f;
    }

    bool isInactive() const;
    bool isMoveNormal() const;
    bool isGravityIgnored() const;

    /* 0x8C */ Type mType;
    /* 0x90 */ f32 mChaseStartDist;
    /* 0x94 */ f32 mChaseEndDist;
    /* 0x98 */ f32 mChaseRotateSpeed;
    /* 0x9C */ f32 mChaseStartAngle;
    /* 0xA0 */ TVec3f mFront;
    /* 0xAC */ TVec3f mUp;
    /* 0xB8 */ TVec3f mBasePos;
    /* 0xC4 */ TVec3f mBaseFront;
    /* 0xD0 */ TVec3f mBaseUp;
    /* 0xDC */ TPos3f mBaseMtx;
    /* 0x10C */ s32 mFreezeTime;
    /* 0x110 */ TVec3f mFreezePos;
    /* 0x11C */ const Nerve* mUnfreezeNerve;
    /* 0x120 */ s32 mChaseInvalidTime;
    /* 0x124 */ s32 mMoveTime;
    /* 0x128 */ TPos3f mEffectMtx;
    /* 0x158 */ HitSensor* mTargetSensor;
    /* 0x15C */ bool mDisableChase;
    /* 0x15D */ bool mIsLinearShot;       // if set, ignore gravity and disable chase
    /* 0x15E */ bool mUseFullSightAngle;  // if set, ignore gravity but use full sight angle to start chase
                                          // In game, this seems to only be used in Drip Drop for the Torpedos
    /* 0x160 */ PartsModel* mPropeller;
    /* 0x164 */ ModelObj* mTorpedoLight;
};

class HomingKillerLauncher : public LiveActor {
public:
    HomingKillerLauncher(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeAppearKiller();
    void exeDeadKiller();

    /* 0x8C */ HomingKiller* mKiller;
};
