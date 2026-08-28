#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/Util/JointController.hpp"

class FirePressureRadiate : public LiveActor {
public:
    FirePressureRadiate(const char*);

    /* 0x08 */ virtual ~FirePressureRadiate();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeRadiateMargin();
    void exeRadiate();
    void exePrepareToRadiate();
    inline void exeWait();
    void exeSyncWait();
    void exeRelax();
    void startWait();
    void startRelax();
    void calcRadiateEffectMtx();
    bool calcJointCannon(TPos3f*, const JointControllerInfo&);

    JointController* mJointController;  // 0x8C
    TMtx34f mRadiateMtx;                // 0x90
    f32 mCannonRotation;                // 0x94
    s32 mWaitTime;                      // 0x98
    s32 mShootTime;                     // 0x9C
    s32 _CC;
    f32 _D0;
    f32 _D4;
    MsgSharedGroup* mGroup;  // 0xD8
    bool _DC;
};
