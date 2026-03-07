#pragma once

#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class BegomanBaby;

struct BegomanSound {
    const char* mSound;
};

class BegomanBase : public LiveActor {
public:
    BegomanBase(const char*);

    virtual void makeActorDead();
    virtual void appear();
    virtual void kill();

    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void startClipped();

    virtual u32 getKind() const = 0;

    virtual bool onTouchElectric(const TVec3f&, const TVec3f&);

    virtual bool setNerveReturn() = 0;
    virtual const Nerve* setNerveLaunch();
    virtual const Nerve* getNerveWait() = 0;

    virtual void addVelocityOnPushedFromElectricRail(const TVec3f&, const TVec3f&);

    virtual bool requestAttack();

    const BegomanSound* getSoundBaby();
    const BegomanSound* getSoundNormal();
    const BegomanSound* getSoundBoss();

    f32 getRailPushVelHBoss() const;
    f32 getRailPushJumpBoss() const;

    void initCore(const JMapInfoIter&, const char*, bool);
    void initEffect(s32);
    void initEffectAndStarPointerBind();
    void initAfterPlacement();
    void initShadow(f32, const char*);
    void initSensor(s32, f32, f32, const char*);
    void initUseSwitchB(const JMapInfoIter&, const MR::FunctorBase&);

    bool calcJointLocator1(TPos3f*, const JointControllerInfo&);
    void calcAnimCore(TPos3f*);

    void exeNoCalcWaitCore(f32, const Nerve*);
    void finishNoCalcWait();
    void exeWaitCore(const MR::ActorMoveParam&, const Nerve*, const Nerve*, const Nerve*);
    void exeSignAttackCore(const MR::ActorMoveParam&, const Nerve*);
    void exePursueCore(const MR::ActorMoveParam&, const Nerve*, const Nerve*, const BegomanSound&, f32);
    void exeTurnCore(const MR::ActorMoveParam&, const Nerve*, const Nerve*, bool);
    void exeBrakeCore(const Nerve*);
    void exeStepBackCore(const MR::ActorMoveParam&, const Nerve*);
    void exeProvokeCore(const MR::ActorMoveParam&, const Nerve*);
    void exeHitReactionCore(const MR::ActorMoveParam&, const Nerve*);
    void exeTiredCore(const MR::ActorMoveParam&, const Nerve*);
    void exeReturnCore(const Nerve*);
    void exeKeepDistanceCore(const Nerve*, const Nerve*, const Nerve*, f32, f32);
    void exeLaunch();
    void exeBindStarPointer();
    void finishBindStarPointer();

    static void launchBegomanCore(LiveActor*, BegomanBase**, s32, f32, f32, f32, const TVec3f*);
    static void launchBegoman(LiveActor*, BegomanBase**, s32, f32, f32, f32, const TVec3f*);
    static void launchBegomanBabyFromGuarder(LiveActor*, BegomanBaby**, s32, f32, f32, f32, const TVec3f*);
    static void launchBegomanBabyLauncher(LiveActor*, BegomanBaby**, s32, f32, f32, f32, const TVec3f*);


    void updateTargetVec();
    void updateRotateY(f32, f32);
    void addVelocityEscapeToSide(f32);
    void calcDirectionAwayFromRail(TVec3f*, const TVec3f&, const TVec3f&);
    void pushedFromElectricRail(HitSensor*, const TVec3f&, const TVec3f&, f32, f32, bool);
    bool checkTouchElectricRail(bool);

    bool reboundPlaneWithEffect(const TVec3f&, f32, f32, const char*);
    void reboundWallAndGround(TVec3f*, bool);

    void preventSwingby(f32);
    void preventSlopeBlow(f32);
    void dampingVerticalAndParallelVelocity(f32, f32);
    void calcBlowReaction(const TVec3f&, const TVec3f&, f32, f32);

    bool isNearInitPos() const;
    bool isFallNextMove(f32, f32);
    bool trySetReturnNerve();
    bool tryAndSetStarPointerBind(const Nerve*);
    bool incAndCheckTiredCounter();
    bool isInWaterAndSetWaterNerve(const Nerve*, TPos3f*);

    JointControlDelegator< BegomanBase >* mBaseDelegator; // _8C

    TVec3f mFaceVec; //0x90
    TVec3f mTargetVec;  // 0x9C

    TVec3f _A8;
    TVec3f _B4;

    TQuat4f _C0;
    TQuat4f _D0;

    s32 mTiredCounter; // _E0
    s32 mElectricCounter; //_E4

    TVec3f mInitPos; //_E8

    AnimScaleController* mScaleControler;
    WalkerStateBindStarPointer* mStarPointBind;

    bool mIsTouchElectricRail;
    bool mCanTrySetReturn;
    bool mInvertSideVec;
};

class BegomanAttackPermitter : public LiveActor {
public: 
    BegomanAttackPermitter(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    bool requestAttack(BegomanBase*);

    void exeWait();
    void exeReceive();
    inline void exePermit();

    BegomanBase* _8C;
    BegomanBase* mBegoman;

    f32 mDistToPlayer;

    bool _98;

};
