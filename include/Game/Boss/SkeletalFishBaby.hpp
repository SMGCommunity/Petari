#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class JointController;
class JointControllerInfo;
class SkeletalFishRailControl;

class SkeletalFishBaby : public LiveActor {
public:
    /// @brief Creates a new `SkeletalFishBaby`.
    /// @param pName A pointer to the null-terminated name of the object.
    SkeletalFishBaby(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeSwim();
    void exeBind();
    void endBind();
    void exeBreak();
    void exeDead();
    bool calcJoint(TPos3f*, const JointControllerInfo&);
    bool damage(const TVec3f&, bool);
    void initRail(const JMapInfoIter&);
    void initJoint();
    void initSensor();
    bool isAttackable() const;
    bool isStarPointerPointing() const;

    /* 0x8C */ JointController* mControllers[4];
    /* 0x9C */ s32* mJointIndicies;
    /* 0xA0 */ f32 mSpeedMax;
    /* 0xA4 */ f32 mSpeed;
    /* 0xA8 */ SkeletalFishRailControl* mRailControl;
    /* 0xAC */ TPos3f _AC;
    /* 0xDC */ f32 mObjArg1;
    /* 0xE0 */ AnimScaleController* mScaleController;
    /* 0xE4 */ LiveActor* mStarPieceTargets[4];
};
