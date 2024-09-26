#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Util/JointController.hpp"

class SkeletalFishRailControl;

class SkeletalFishBaby : public LiveActor {
public:
    SkeletalFishBaby(const char *);

    virtual ~SkeletalFishBaby();
    virtual void init(const JMapInfoIter &);
    virtual void calcAnim();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeSwim();
    void exeBind();
    void exeBreak();
    void exeDead();
    bool calcJoint(TPos3f *, const JointControllerInfo &);
    bool damage(const TVec3f &, bool);
    void initRail(const JMapInfoIter &);
    void initJoint();
    void initSensor();
    bool isAttackable() const;
    bool isStarPointerPointing() const;

    void exeOnEndBind() {
        mScaleController->startAnim();
        MR::deleteEffect(this, "StarPointerHolder");
    }

    JointController* mControllers[4];               // 0x8C
    s32* mJointIndicies;                            // 0x9C
    f32 _A0;
    f32 _A4;
    SkeletalFishRailControl* mRailControl;          // 0xA8
    TPos3f _AC;
    f32 _DC;
    AnimScaleController* mScaleController;          // 0xE0 
    LiveActor* mStarPieceTargets[4];                // 0xE4
};

namespace {
    NERVE_DECL(SkeletalFishBabyNrvSwim, SkeletalFishBaby, SkeletalFishBaby::exeSwim);
    NERVE_DECL_ONEND(SkeletalFishBabyNrvBind, SkeletalFishBaby, SkeletalFishBaby::exeBind, SkeletalFishBaby::exeOnEndBind);
    NERVE_DECL(SkeletalFishBabyNrvBreak, SkeletalFishBaby, SkeletalFishBaby::exeBreak);
    NERVE_DECL(SkeletalFishBabyNrvDead, SkeletalFishBaby, SkeletalFishBaby::exeDead);
};