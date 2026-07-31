#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CameraTargetDemoActor;
class FixedPosition;
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

    /* 0x08C */ FixedPosition* mFixedPos;
    /* 0x090 */ ThrowingIce* mThrowingIce;
    /* 0x094 */ Meramera* mMeramera;
    /* 0x098 */ SpinPullParticleCallBack* mSpinParticle;
    /* 0x09C */ MR::Vector< MR::AssignableArray< ThrowingIce* > > mIce;
    /* 0x0A8 */ LiveActor* _A8;
    /* 0x0AC */ LiveActor* _AC;
    /* 0x0B0 */ TVec3f _B0;
    /* 0x0BC */ TVec3f _BC;
    /* 0x0C8 */ TVec3f _C8;
    /* 0x0D4 */ TVec3f _D4;
    /* 0x0E0 */ s32 _E0;
    /* 0x0E4 */ s32 _E4;
    /* 0x0E8 */ u32 _E8;
    /* 0x0EC */ s32 _EC;
    /* 0x0F0 */ s32 _F0;
    /* 0x0F4 */ Meramera** mModelArray;
    /* 0x0F8 */ CameraTargetDemoActor* mCameraTarget;
    /* 0x0FC */ JointControlDelegator< IceMerameraKing >* mJointController;
    /* 0x100 */ TQuat4f _100;
    /* 0x110 */ TVec3f _110;
    /* 0x11C */ f32 _11C;
    /* 0x120 */ bool _120;
    /* 0x121 */ bool _121;
};

class IceMerameraKingShockWave : public ModelObj {
public:
    IceMerameraKingShockWave();

    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
};
