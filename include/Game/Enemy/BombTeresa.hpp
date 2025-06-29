#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointController.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TVec.hpp"


class BombTeresa : public LiveActor {
public:
    BombTeresa(const char *);

    ~BombTeresa();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPush(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);
 
    void initTongue();
    void initSensor();
    void initFromJMapParam(const JMapInfoIter&);
    void exeAppear();
    void exeShadowAppear();
    void exeBallAppear();
    void exeWait();
    void exeWander();
    void exeChase();
    void exeAttackTongueFailed();
    void exeDirectTackleSign();
    void exeDirectTackle();
    void exeDrift();
    void exeDriftRelease();
    void exeExplosion();
    void exeShock();
    void exeDisperse();
    void exeReadyRestart();
    void exeBindStarPointer();
    void tryCheseEnd();
    void tryDirectTackle();
    void tryAbortDrift();
    void tryDriftRelease();
    void tryExplosion();
    void tryRevival();
    bool requestDrift();
    bool requestDisperse();
    void appearNormal(const TVec3f &, const TVec3f &);
    void appearShadow(const TVec3f &, const TVec3f &);
    bool rootTongueMtxCallBack(TPos3f*, const JointControllerInfo&);
    bool endTongueMtxCallBack(TPos3f*, const JointControllerInfo&);
    void updateNormalVelocity();
    void updateDriftReleaseVelocity();
    bool isTouchTongue() const;
    bool isEnableDrift() const;
    bool isEnablePointBind() const;
    bool isEnableDisperse() const;
    bool isEnableHitExplosionToPlayer() const;
    bool isEnableHitExplosionToEnemy() const;
    bool isEnableHitExplosion() const;
    bool isEnableStarPieceAttack() const;
    bool isEnableShockWave() const;
    void addTeresaSpinPullVelocity(f32);

    inline void applyScale(f32 scale) {
        mScale.x *= scale;
        mScale.y *= scale;
        mScale.z *= scale;
    }

    JointControlDelegator<BombTeresa>* mJointDelegator;
    JointControlDelegator<BombTeresa>* mJointDelegator2;
    AnimScaleController* mScaleController;
    WalkerStateBindStarPointer* mBindStarPointer;
    TQuat4f _9C;
    TVec3f _AC;
    TVec3f _B8;
    TVec3f _C4;
    TVec3f _D0;
    f32 _DC;
    f32 _E0;
    f32 _E4;
    s32 _E8;
    bool  _EC;
    bool mDisableRespawning;
    bool  _EE;
    bool  _EF;

};
