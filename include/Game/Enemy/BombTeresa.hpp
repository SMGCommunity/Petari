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
    BombTeresa(const char*);

    virtual ~BombTeresa();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
    void exeBindStarPointer();
    void endBindStarPointer();
    void exeExplosion();
    void exeShock();
    void exeDisperse();
    void exeReadyRestart();
    bool tryCheseEnd();
    bool tryDirectTackle();
    bool tryAbortDrift();
    bool tryDriftRelease();
    bool tryDriftReleaseEnd();
    bool tryExplosion();
    bool tryRevival();
    bool requestDrift();
    bool requestDisperse();
    bool appearNormal(const TVec3f&, const TVec3f&);
    bool appearShadow(const TVec3f&, const TVec3f&);
    bool rootTongueMtxCallBack(TPos3f*, const JointControllerInfo&);
    bool endTongueMtxCallBack(TPos3f*, const JointControllerInfo&);
    void updateNormalVelocity();
    void updateDriftReleaseVelocity();
    bool isTouchTongue() const NO_INLINE;
    bool isEnableDrift() const NO_INLINE;
    bool isEnablePointBind() const NO_INLINE;
    bool isEnableDisperse() const NO_INLINE;
    bool isEnableHitExplosionToPlayer() const NO_INLINE;
    bool isEnableHitExplosionToEnemy() const NO_INLINE;
    bool isEnableHitExplosion() const NO_INLINE;
    bool isEnableStarPieceAttack() const NO_INLINE;
    bool isEnableShockWave() const NO_INLINE;
    void addTeresaSpinPullVelocity(f32);

    inline void applyScale(f32 scale) {
        mScale.x *= scale;
        mScale.y *= scale;
        mScale.z *= scale;
    }

    inline bool isOutOfRange(f32 dist) { return !MR::isNearPlayer(this, dist); }
    /*0x8C*/ JointControlDelegator< BombTeresa >* mJointDelegator;
    /*0x90*/ JointControlDelegator< BombTeresa >* mJointDelegator2;
    /*0x94*/ AnimScaleController* mScaleController;
    /*0x98*/ WalkerStateBindStarPointer* mBindStarPointer;
    TQuat4f _9C;
    TVec3f _AC;
    TVec3f _B8;
    TVec3f _C4;
    TVec3f _D0;
    f32 _DC;
    f32 _E0;
    f32 _E4;
    s32 _E8;
    bool _EC;
    /*0xED*/ bool mDisableRespawning;
    bool _EE;
    bool _EF;
};
