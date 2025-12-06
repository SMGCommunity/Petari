#pragma once

#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/HitSensorKeeper.hpp"
#include "Game/LiveActor/LiveActorFlag.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ActorAnimKeeper;
class ActorLightCtrl;
class ActorPadAndCameraCtrl;
class AudAnmSoundObject;
class Binder;
class CollisionParts;
class RailRider;
class ShadowControllerList;
class StarPointerTarget;

/// @brief The basis of a drawable actor that can contain states (see: Nerve)
class LiveActor : public NameObj {
public:
    /// @brief Creates a new `LiveActor`.
    /// @param pName The pointer to the null-terminated name of the actor.
    LiveActor(const char* pName);

    /// @brief Destroys the `LiveActor`.
    virtual ~LiveActor() {}

    /// @brief Intializes the `LiveActor` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter) override;

    virtual void movement();
    virtual void calcAnim();
    virtual void calcViewAndEntry();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual bool receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    /// @brief Gets the base matrix of the model used for the actor.
    /// @returns A MtxPtr to the base matrix. NULL if there is no model present.
    virtual MtxPtr getBaseMtx() const;
    virtual MtxPtr getTakingMtx() const;
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    /// @brief Calculates and sets the base matrix of the actor.
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgTake(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgTaken(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgThrow(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgApart(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void calcAnmMtx();
    void setNerve(const Nerve* pNerve);
    bool isNerve(const Nerve* pNerve) const;
    s32 getNerveStep() const;

    /// @brief Gets a sensor.
    /// @param pSensorName The name of the sensor to get.
    /// @returns The sensor that contains the name given. Returns the result of HitSensorKeeper::getSensor or NULL if there is no HitSensorKeeper
    /// created.
    HitSensor* getSensor(const char* pSensorName) const;
    void initModelManagerWithAnm(const char* pModelArcName, const char* pAnimArcName, bool);
    void initNerve(const Nerve*);
    void initHitSensor(int);
    void initBinder(f32, f32, u32);
    void initRailRider(const JMapInfoIter& rIter);
    void initEffectKeeper(int, const char*, bool);
    void initSound(int, bool);
    void initShadowControllerList(u32);
    void initActorCollisionParts(const char*, HitSensor*, ResourceHolder*, MtxPtr, bool, bool);
    void initStageSwitch(const JMapInfoIter& rIter);
    void initActorStarPointerTarget(f32, const TVec3f*, MtxPtr, TVec3f);
    void initActorLightCtrl();
    void addToSoundObjHolder();
    void updateBinder();

    /// @brief The three-dimensional location vector.
    /* 0x0C */ TVec3f mPosition;

    /// @brief The three-dimensional rotation vector, in degrees.
    /* 0x18 */ TVec3f mRotation;

    /// @brief The three-dimensional scale vector.
    /* 0x24 */ TVec3f mScale;

    /// @brief The three-dimensional velocity vector, in units per step.
    /* 0x30 */ TVec3f mVelocity;

    /// @brief The three-dimensional gravity unit vector.
    /* 0x3C */ TVec3f mGravity;

    /// @brief A pointer to a `ModelManager` instance, used for drawing a 3D model.
    /* 0x48 */ ModelManager* mModelManager;

    /// @brief A pointer to a `ActorAnimKeeper` instance, used for storing and playing model animations.
    /* 0x4C */ ActorAnimKeeper* mAnimKeeper;

    /* 0x50 */ Spine* mSpine;

    /// @brief A pointer to a `HitSensorKeeper` instance, used for storing `HitSensor` instances.
    /* 0x54 */ HitSensorKeeper* mSensorKeeper;

    /* 0x58 */ Binder* mBinder;

    /// @brief A pointer to a `RailRider` instance, used for allowing the actor to ride on paths.
    /* 0x5C */ RailRider* mRailRider;
    /* 0x60 */ EffectKeeper* mEffectKeeper;
    /* 0x64 */ AudAnmSoundObject* mSoundObject;

    /// @brief Flags relating to drawing, animation calculation, etc.
    /* 0x68 */ LiveActorFlag mFlag;

    /* 0x74 */ ShadowControllerList* mShadowControllerList;
    /* 0x78 */ CollisionParts* mCollisionParts;

    /// @brief A pointer to a `StageSwitchCtrl` instance, used for activating and deactivating switches.
    /* 0x7C */ StageSwitchCtrl* mStageSwitchCtrl;

    /* 0x80 */ StarPointerTarget* mStarPointerTarget;
    /* 0x84 */ ActorLightCtrl* mActorLightCtrl;
    /* 0x88 */ ActorPadAndCameraCtrl* mCameraCtrl;
};
