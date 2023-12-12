#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/LiveActorGroupArray.h"
#include "Game/Util/JointController.h"

class PressureMessenger : public LiveActor {
public:
    PressureMessenger(MsgSharedGroup *, const char *);

    virtual ~PressureMessenger();
    virtual void init(const JMapInfoIter &);
    
    void exeSync();

    MsgSharedGroup* mSharedGroup;   // _8C
    s32 _90;
};

class PressureBase : public LiveActor {
public:
    PressureBase(const char *);

    virtual ~PressureBase() {

    }

    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);
    virtual void initBullet(const JMapInfoIter &);
    virtual bool shotBullet(f32);

    void exeBound();
    void exeWait();
    void exePrepareToShot();
    void exeShot();
    void startWait();
    void startRelax();
    bool calcJointCannonV(TPos3f *, const JointControllerInfo &);
    bool isShotTypeOnGravity() const;
    bool isShotTypeFollow() const;

    JointControlDelegator<PressureBase>* mJointController;  // _8C
    TVec3f mFront;                                          // _90
    f32 _9C;
    f32 mNozzleRotation;                                    // _A0
    s32 mWaitTime;                                          // _A4
    f32 mBallSpeed;                                         // _A8
    s32 mShotType;                                          // _AC
    u8 _B0;
    u8 _B1;
    u8 _B2;
    u8 _B3;
    PressureMessenger* mMessenger;                          // _B4
    MsgSharedGroup* mGroup;                                 // _B8
    s8 _BC;
};

namespace NrvPressureMessenger {
    NERVE(PressureMessengerNrvSync);

};

namespace NrvPressureBase {
    NERVE(PressureBaseNrvRelaxStart);
    NERVE(PressureBaseNrvWaitStart);
    NERVE(PressureBaseNrvRelax);
    NERVE(PressureBaseNrvSyncWait);
    NERVE(PressureBaseNrvFirstWait);
    NERVE(PressureBaseNrvWait);
    NERVE(PressureBaseNrvPrepareToShot);
    NERVE(PressureBaseNrvShot);
};