#pragma once

#include "Game/Boss/SkeletalFishBossInfo.hpp"
#include "Game/LiveActor/PartsModel.hpp"

class CameraTargetMtx;
class JointController;
class JointControllerInfo;
class ModelObj;
class SkeletalFishGuard;
class SkeletalFishGuardHolder;
class SkeletalFishBossScarFlash;
class SkeletalFishBossHead;
class SkeletalFishRailControl;
class SkeletalFishBossBattleDirector;
class SkeletalFishBossRail;

class SkeletalFishBoss : public LiveActor {
public:
    struct SensorToCollider {
        const char* mJointName;
        const char* mColliderName;
    };

    struct JointToShadow {
        const char* mName;
        const char* mJointName;
        const char* mShadowName;
    };

    struct HitPos {
        const char* mName;
        const char* mSensorName;
        Vec mOffset;
    };

    typedef void (SkeletalFishBoss::*SceneFunc)(void);

    SkeletalFishBoss(const char*);

    virtual ~SkeletalFishBoss() {
    }

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    bool calcJoint(TPos3f*, const JointControllerInfo&);
    void damage(const HitSensor*, const TVec3f&);
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
    void notifyAttack(SkeletalFishGuard*);
    LiveActor* getCurrentRail();
    f32 getBodyThickness() const;
    SkeletalFishBossHead* getHeadActor() const;
    void initLevelStatus();
    void initJoint();
    void initCollision();
    void initHead();
    void initScarFlash();
    void initBreakModel();
    void initSwitch(const JMapInfoIter&);
    void createGuards();
    void initShadow();
    void initCamera();
    void validateCollision();
    void invalidateCollision();
    void getMouthSensorCenterPos(TVec3f&, f32) const;
    void updateCollision();
    void powerUp();
    void startDamageAnim();
    void calcPlanetCenterPosition();
    bool calcGravityCenter(TVec3f*, const TVec3f&, const TVec3f&);
    void resetRail();
    void updateBgm();
    LiveActor* getCurrentBossRail();
    void stopScene(const char*, const Nerve*, SceneFunc);
    void startCamera(const char*);
    void resetCamera();
    void playDamageBrk();
    void startAppearDemo();
    void endAppearDemo();
    void startPowerUpDemo();
    void endPowerUpDemo();
    void startDeadDemo();
    void endBreakDemo();
    bool isEnableToBeDamaged() const;

    SkeletalFishBossInfo::LevelStatus* getLevelStatus() const {
        return mBossInfo->getLevelStatus(_110);
    }

    /* 0x8C */ JointController* mControllers[0xD];
    /* 0xC0 */ s32* mJointIndicies;
    /* 0xC4 */ CollisionParts** mPartsArray;
    /* 0xC8 */ SkeletalFishBossHead* mBossHead;
    /* 0xCC */ SkeletalFishBossScarFlash* mScarFlash;
    /* 0xD0 */ ModelObj* mBreakModel;
    /* 0xD4 */ const HitSensor* _D4;
    /* 0xD8 */ TPos3f _D8;
    /* 0x108 */ SkeletalFishRailControl* mRailControl;
    /* 0x10C */ SkeletalFishBossBattleDirector* mBossDirector;
    /* 0x110 */ s32 _110;
    /* 0x114 */ s32 _114;
    /* 0x118 */ u32 _118;
    /* 0x11C */ CameraTargetMtx* mCameraTargetMtx;
    /* 0x120 */ TPos3f _120;
    /* 0x150 */ TPos3f _150;
    /* 0x180 */ TVec3f _180;
    /* 0x18C */ SceneFunc mSceneFunc;
    /* 0x198 */ const char* mCurScene;
    /* 0x19C */ const Nerve* mSceneNerve;
    /* 0x1A0 */ s32 _1A0;
    /* 0x1A4 */ s32 _1A4;
    /* 0x1A8 */ SkeletalFishBossInfo* mBossInfo;
    /* 0x1AC */ SkeletalFishGuardHolder* mGuardHolder;
    /* 0x1B0 */ s32 _1B0;
    /* 0x1B4 */ s32 _1B4;
};

class SkeletalFishBossScarFlash : public PartsModel {
public:
    SkeletalFishBossScarFlash(LiveActor*);

    virtual ~SkeletalFishBossScarFlash() {
    }

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
};

class SkeletalFishBossHead : public PartsModel {
public:
    SkeletalFishBossHead(LiveActor*);

    virtual ~SkeletalFishBossHead() {
    }

    virtual void movement();
    virtual void calcAnim();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void updateCollisionMtx();
    void createSubModel();

    /* 0x9C */ TMtx34f _9C;
    /* 0xCC */ CollisionParts* _CC;
    /* 0xD0 */ TMtx34f _D0;
    /* 0x100 */ CollisionParts* _100;
    /* 0x104 */ ModelObj* mLightModels[2];
    /* 0x10C */ ModelObj* mBloomModels[2];
    /* 0x114 */ u8 _114;
};
