#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class JointController;
class JointControllerInfo;
class MsgSharedGroup;

class FirePressureRadiate : public LiveActor {
public:
    FirePressureRadiate(const char*);

    virtual ~FirePressureRadiate();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    /* 0x8C */ JointController* mJointController;
    /* 0x90 */ TPos3f mRadiateMtx;
    /* 0xC0 */ f32 mCannonRotation;
    /* 0xC4 */ s32 mWaitTime;
    /* 0xC8 */ s32 mShootTime;
    /* 0xCC */ s32 _CC;
    /* 0xD0 */ f32 _D0;
    /* 0xD4 */ f32 _D4;
    /* 0xD8 */ MsgSharedGroup* mGroup;
    /* 0xDC */ bool _DC;
};
