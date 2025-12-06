#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"

class CameraTargetDemoActor;
template < typename T >
class JointControlDelegator;
class JointControllerInfo;
class Meramera;
class SpinPullParticleCallBack;
class ThrowingIce;

class IceMerameraKing : public LiveActor {
public:
    IceMerameraKing(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeSearch();
    void exeThrow();
    void endThrow();
    void tearDownThrow();
    void exeExtinguish();
    void exeEscape();
    void endEscape();
    void exeEscapeJump();
    void exeDamage();
    void exePreRecover();
    void exeRecover();
    void exePreAttack();
    void exeAttack();
    void exeAttackAfter();
    void exeAngryDemo();
    void exeDeathDemoWait();
    void exeDeathDemo();
    void addVelocityToInitPos();
    bool isDeadAllIce();
    bool isEnableThrow();
    ThrowingIce* getDeadWeaponAndAppear();
    bool calcJoint(TPos3f*, const JointControllerInfo&);

    FixedPosition* mFixedPos;  //_8C
    ThrowingIce* _90;
    Meramera* _94;
    SpinPullParticleCallBack* mSpinParticle;  //_98
    ThrowingIce** mActor;                     //_9C
    s32 _A0;
    s32 _A4;
    LiveActor* _A8;
    LiveActor* _AC;
    TVec3f _B0;
    TVec3f _BC;
    TVec3f _C8;
    TVec3f _D4;
    s32 _E0;
    s32 _E4;
    u32 _E8;
    s32 _EC;
    s32 _F0;
    MR::Vector< MR::AssignableArray< ThrowingIce* > > mModelArray;  //_F4
    TQuat4f _100;
    TVec3f _110;
    f32 _11C;
    bool _120;
    bool _121;
    CameraTargetDemoActor* _F8;
    JointControlDelegator< IceMerameraKing >* mJointController;  //_FC
};

class IceMerameraKingShockWave : public ModelObj {
public:
    IceMerameraKingShockWave();

    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
};
