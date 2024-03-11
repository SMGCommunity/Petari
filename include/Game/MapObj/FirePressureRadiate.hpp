#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/Util/JointController.hpp"

class FirePressureRadiate : public LiveActor {
public:
    FirePressureRadiate(const char *);

    virtual ~FirePressureRadiate();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor *);
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeRadiateMargin();
    void exeRadiate();
    void exePrepareToRadiate();
    inline void exeWait();
    void exeSyncWait();
    void exeRelax();
    void startWait();
    void startRelax();
    void calcRadiateEffectMtx();
    bool calcJointCannon(TPos3f *, const JointControllerInfo &);

    JointController* mJointController;      // _8C
    TMtx34f mRadiateMtx;                    // _90
    f32 mCannonRotation;                    // _94
    s32 mWaitTime;                          // _98
    s32 mShootTime;                         // _9C
    s32 _CC;
    f32 _D0;
    f32 _D4;
    MsgSharedGroup* mGroup;                 // _D8
    bool _DC;
};

namespace NrvFirePressureRadiate {
    NERVE_DECL(FirePressureRadiateNrvRelax, FirePressureRadiate, FirePressureRadiate::exeRelax);
    NERVE_DECL(FirePressureRadiateNrvSyncWait, FirePressureRadiate, FirePressureRadiate::exeSyncWait);
    NERVE_DECL(FirePressureRadiateNrvWait, FirePressureRadiate, FirePressureRadiate::exeWait);
    NERVE_DECL(FirePressureRadiateNrvPrepareToRadiate, FirePressureRadiate, FirePressureRadiate::exePrepareToRadiate);
    NERVE_DECL(FirePressureRadiateNrvRadiate, FirePressureRadiate, FirePressureRadiate::exeRadiate);
    NERVE_DECL(FirePressureRadiateNrvRadiateMargin, FirePressureRadiate, FirePressureRadiate::exeRadiateMargin);
};