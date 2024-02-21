#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/JointController.hpp"

class SkeletalFishGuard;
class SkeletalFishGuardHolder;
class SkeletalFishBossScarFlash;
class SkeletalFishBossHead;
class SkeletalFishBossInfo;
class SkeletalFishRailControl;
class SkeletalFishBossBattleDirector;

class SkeletalFishBoss : public LiveActor {
public:

    typedef void (SkeletalFishBoss::*SceneFunc)(const void *, void *);

    virtual ~SkeletalFishBoss();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    bool calcJoint(TPos3f *, const JointControllerInfo &);
    void damage(const HitSensor *, const TVec3f &);
    void exeSwim();
    void exeOpen();
    void exeOpenWait();
    void exeClose();
    void exeBite();
    void exeDamage();
    void exeDown();
    void exeDeadDamage();
    void exeDead();
    void exeAppearWait();
    void exeAppearDemo();
    void exePowerUpDemo();
    void exeDeadDemo();
    void exeBreakDemo();
    void exeDemoWait();
    void notifyAttack(SkeletalFishGuard *);
    LiveActor* getCurrentRail();
    f32 getBodyThickness() const;
    SkeletalFishBossHead* getHeadActor() const;
    void initLevelStatus();
    void initJoint();
    void initCollision();
    void initHead();
    void initScarFlash();
    void initBreakModel();
    void initSwitch();
    void createGuards();
    void initShadow();
    void initCamera();
    void validateCollision();
    void invalidateCollision();
    void getMouthSensorCenterPos(TVec3f &, f32);
    void updateCollision();
    void powerUp();
    void startDamageAnim();
    void calcPlanetCenterPosition();
    void calcGravityCenter(TVec3f *, const TVec3f &, const TVec3f &);
    void resetRail();
    void updateBgm();
    LiveActor* getCurrentBossRail();
    void stopScene(const char *, const Nerve *, SceneFunc);
    void startCamera(const char *);
    void resetCamera();
    void playDamageBrk();
    void startAppearDemo();
    void endAppearDemo();
    void startPowerUpDemo();
    void endPowerUpDemo();
    void startDeadDemo();
    void endBreakDemo();
    bool isEnableToBeDamaged() const;

    JointController* mControllers[0xD];
    u32 _C0;
    CollisionParts** mPartsArray;                   // _C4
    SkeletalFishBossHead* mBossHead;                // _C8
    SkeletalFishBossScarFlash* mScarFlash;              // _CC
    ModelObj* mBreakModel;                          // _D0
    HitSensor* _D4;
    TPos3f _D8;
    SkeletalFishRailControl* mRailControl;          // _108
    SkeletalFishBossBattleDirector* mBossDirector;  // _10C
    s32 _110;
    u32 _114;
    u32 _118;
    u32 _11C;
    TPos3f _120;
    TPos3f _150;
    f32 _180;
    f32 _184;
    f32 _188;
    SceneFunc mSceneFunc;                           // _18C
    u32 _198;
    Nerve* _19C;
    s32 _1A0;
    s32 _1A4;
    SkeletalFishBossInfo* mBossInfo;                // _1A8
    SkeletalFishGuardHolder* mGuardHolder;          // _1AC
    u32 _1B0;
    s32 _1B4;
};

class SkeletalFishBossScarFlash : public PartsModel {
public:
    SkeletalFishBossScarFlash(LiveActor *);

    virtual ~SkeletalFishBossScarFlash();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void control();
};

class SkeletalFishBossHead : public PartsModel {
public:
    SkeletalFishBossHead(LiveActor *);

    virtual ~SkeletalFishBossHead();
    virtual void movement();
    virtual void calcAnim();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    
    void updateCollisionMtx();
    void createSubModel();

    TMtx34f _9C;
    CollisionParts* _CC;
    TMtx34f _D0;
    CollisionParts* _100;
    ModelObj** _104;
    u32 _108;
    ModelObj** _10C;
    u8 _114;
};

namespace {
    NERVE_DECL(SkeletalFishBossNrvSwim, SkeletalFishBoss, SkeletalFishBoss::exeSwim);
    NERVE_DECL(SkeletalFishBossNrvOpen, SkeletalFishBoss, SkeletalFishBoss::exeOpen);
    NERVE_DECL(SkeletalFishBossNrvOpenWait, SkeletalFishBoss, SkeletalFishBoss::exeOpenWait);
    NERVE_DECL(SkeletalFishBossNrvClose, SkeletalFishBoss, SkeletalFishBoss::exeClose);
    NERVE_DECL(SkeletalFishBossNrvBite, SkeletalFishBoss, SkeletalFishBoss::exeBite);
    NERVE_DECL(SkeletalFishBossNrvDamage, SkeletalFishBoss, SkeletalFishBoss::exeDamage);
    NERVE_DECL(SkeletalFishBossNrvDown, SkeletalFishBoss, SkeletalFishBoss::exeDown);
    NERVE_DECL(SkeletalFishBossNrvDeadDamage, SkeletalFishBoss, SkeletalFishBoss::exeDeadDamage);
    NERVE_DECL(SkeletalFishBossNrvDead, SkeletalFishBoss, SkeletalFishBoss::exeDead);
    NERVE_DECL(SkeletalFishBossNrvAppearWait, SkeletalFishBoss, SkeletalFishBoss::exeAppearWait);
    NERVE_DECL(SkeletalFishBossNrvAppearDemo, SkeletalFishBoss, SkeletalFishBoss::exeAppearDemo);
    NERVE_DECL(SkeletalFishBossNrvPowerUpDemo, SkeletalFishBoss, SkeletalFishBoss::exePowerUpDemo);
    NERVE_DECL(SkeletalFishBossNrvDeadDemo, SkeletalFishBoss, SkeletalFishBoss::exeDeadDemo);
    NERVE_DECL(SkeletalFishBossNrvBreakDemo, SkeletalFishBoss, SkeletalFishBoss::exeBreakDemo);
    NERVE_DECL(SkeletalFishBossNrvDemoWait, SkeletalFishBoss, SkeletalFishBoss::exeDemoWait);
};