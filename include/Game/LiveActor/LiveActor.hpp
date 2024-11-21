#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActorFlag.hpp"
#include "Game/LiveActor/ActorAnimKeeper.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/HitSensorKeeper.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class ActorPadAndCameraCtrl;
class AudAnmSoundObject;
class Binder;
class CollisionParts;
class ShadowControllerList;
class StarPointerTarget;
class RailRider;

/// @brief The basis of a drawable actor that can contain states (see: Nerve)
class LiveActor : public NameObj {
public:
    /// @brief Constructs a new LiveActor instance.
    /// @param pName The new name assigned to the LiveActor.
    LiveActor(const char *pName);

    virtual inline ~LiveActor();

    virtual void init(JMapInfoIter const &);
    virtual void movement();
    virtual void calcAnim();
    virtual void calcViewAndEntry();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual bool receiveMessage(u32, HitSensor *, HitSensor *);

    /// @brief Gets the base matrix of the model used for the actor.
    /// @returns A MtxPtr to the base matrix. NULL if there is no model present.
    virtual MtxPtr getBaseMtx() const;
    virtual MtxPtr getTakingMtx() const;
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    /// @brief Calculates and sets the base matrix of the actor.
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor *);
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPush(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual u32 receiveMsgTake(HitSensor *, HitSensor *);
    virtual u32 receiveMsgTaken(HitSensor *, HitSensor *);
    virtual u32 receiveMsgThrow(HitSensor *, HitSensor *);
    virtual u32 receiveMsgApart(HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void calcAnmMtx();
    void setNerve(const Nerve *);
    bool isNerve(const Nerve *) const;
    s32 getNerveStep() const;

    /// @brief Gets a sensor.
    /// @param pSensorName The name of the sensor to get.
    /// @returns The sensor that contains the name given. Returns the result of HitSensorKeeper::getSensor or NULL if there is no HitSensorKeeper created.
    HitSensor* getSensor(const char *pSensorName) const;
    void initModelManagerWithAnm(const char *, const char *, bool);
    void initNerve(const Nerve *);
    void initHitSensor(int);
    void initBinder(f32, f32, u32);
    void initRailRider(const JMapInfoIter &);
    void initEffectKeeper(int, const char *, bool);
    void initSound(int, bool);
    void initShadowControllerList(u32);
    void initActorCollisionParts(const char *, HitSensor *, ResourceHolder *, MtxPtr, bool, bool);
    void initStageSwitch(const JMapInfoIter &);
    void initActorStarPointerTarget(f32, const TVec3f *, MtxPtr, TVec3f);
    void initActorLightCtrl();
    void addToSoundObjHolder();
    void updateBinder();

    inline bool isBinderValid(Binder *pBinder) {
        return pBinder;
    }
    
    inline TVec3f* getPosPtr() {
        return &mPosition;
    }

    /* 0xC */       TVec3f mPosition;                       ///< 3D vector of the actor's position.
    /* 0x18 */      TVec3f mRotation;                       ///< 3D vector of the actor's rotation.
    /* 0x24 */      TVec3f mScale;                          ///< 3D vector of the actor's scale.
    /* 0x30 */      TVec3f mVelocity;                       ///< 3D vector of the actor's velocity.
    /* 0x3C */      TVec3f mGravity;                        ///< 3D vector of the actor's gravity.
    /* 0x48 */      ModelManager* mModelManager;            ///< A pointer to a ModelManager instance, used for drawing a 3D model.
    /* 0x4C */      ActorAnimKeeper* mAnimationKeeper;      ///< A pointer to a ActorAnimKeeper instance, used for storing and playing model animations.
    Spine* mSpine;                          // 0x50
    /* 0x54 */      HitSensorKeeper* mSensorKeeper;         ///< A pointer to a HitSensorKeeper instance, used for storing HitSensor instances.
    Binder* mBinder;                        // 0x58
    /* 0x5C */      RailRider* mRailRider;                  ///< A pointer to a RailRider instance, used for allowing the actor to ride on paths.
    EffectKeeper* mEffectKeeper;            // 0x60
    AudAnmSoundObject* mSoundObject;        // 0x64
    /* 0x68 */      LiveActorFlag mFlags;                   ///< Flags relating to drawing, animation calculation, etc.
    ShadowControllerList* mShadowList;      // 0x74
    CollisionParts* mCollisionParts;        // 0x78
    /* 0x7C */      StageSwitchCtrl* mStageSwitchCtrl;      ///< A pointer to a StageSwitchCtrl instance, used for activating and deactivating switches.
    StarPointerTarget* mStarPointerTarget;  // 0x80
    ActorLightCtrl* mActorLightCtrl;        // 0x84
    ActorPadAndCameraCtrl* mCameraCtrl;     // 0x88
};
