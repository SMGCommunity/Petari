#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class JointController;
class JointControllerInfo;
class SkeletalFishRailControl;

class SkeletalFishBaby : public LiveActor {
public:
    SkeletalFishBaby(const char*);

    /* 0x08 */ virtual ~SkeletalFishBaby();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    JointController* mControllers[4];  // 0x8C
    s32* mJointIndicies;               // 0x9C
    f32 _A0;
    f32 _A4;
    SkeletalFishRailControl* mRailControl;  // 0xA8
    TPos3f _AC;
    f32 _DC;
    AnimScaleController* mScaleController;  // 0xE0
    LiveActor* mStarPieceTargets[4];        // 0xE4
};
