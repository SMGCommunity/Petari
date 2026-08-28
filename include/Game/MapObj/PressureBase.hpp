#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/Util/JointController.hpp"

class PressureMessenger : public LiveActor {
public:
    PressureMessenger(MsgSharedGroup*, const char*);

    /* 0x08 */ virtual ~PressureMessenger();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    void exeSync();

    MsgSharedGroup* mSharedGroup;  // 0x8C
    s32 _90;
};

class PressureBase : public LiveActor {
public:
    PressureBase(const char*);

    /* 0x08 */ virtual ~PressureBase() {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void initBullet(const JMapInfoIter&);
    /* 0x7C */ virtual bool shotBullet(f32);

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
