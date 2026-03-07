#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/Util/JointController.hpp"

class PressureMessenger : public LiveActor {
public:
    PressureMessenger(MsgSharedGroup*, const char*);

    virtual ~PressureMessenger();
    virtual void init(const JMapInfoIter&);

    void exeSync();

    MsgSharedGroup* mSharedGroup;  // 0x8C
    s32 _90;
};

class PressureBase : public LiveActor {
public:
    PressureBase(const char*);

    virtual ~PressureBase() {}

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void initBullet(const JMapInfoIter&);
    virtual bool shotBullet(f32);

    void exeBound();
    void exeRelax();
    void exeSyncWait();
    void exeFirstWait();
    void exeWait();
    void exePrepareToShot();
    void exeShot();
    void startWait();
    void startRelax();
    bool calcJointCannonV(TPos3f*, const JointControllerInfo&);
    bool isShotTypeOnGravity() const;
    bool isShotTypeFollow() const;

    JointControlDelegator< PressureBase >* mJointController;  // 0x8C
    TVec3f mFront;                                            // 0x90
    f32 _9C;
    f32 mNozzleRotation;  // 0xA0
    s32 mWaitTime;        // 0xA4
    f32 mBallSpeed;       // 0xA8
    s32 mShotType;        // 0xAC
    u8 _B0;
    u8 _B1;
    u8 _B2;
    u8 _B3;
    PressureMessenger* mMessenger;  // 0xB4
    MsgSharedGroup* mGroup;         // 0xB8
    s8 _BC;
};
