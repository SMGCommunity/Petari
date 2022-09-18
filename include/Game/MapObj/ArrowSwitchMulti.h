#pragma once

#include "Game/LiveActor/LiveActor.h"

class ArrowSwitchTarget : public NameObj {
public:
    ArrowSwitchTarget(const char *);

    virtual ~ArrowSwitchTarget();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();

    void onTarget();
    void offTarget();

    JMapIdInfo* mJMapIDInfo;                // _C
    StageSwitchCtrl* mStageSwitchCtrl;      // _10
    s32 mTargetIdx;                         // _14
};

class ArrowSwitchMulti : public LiveActor {
public:
    ArrowSwitchMulti(const char *);

    virtual ~ArrowSwitchMulti();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void registerTarget(ArrowSwitchTarget *);
    bool requestPunch(HitSensor *, HitSensor *);
    void exeWait();
    void exeRotate();

    JMapIdInfo* mIDInfo;                    // _8C
    ArrowSwitchTarget* mTargetArray[4];     // _90
    f32 _A0;
    f32 _A4;
    u32 _A8;
    s32 _AC;
    u8 _B0;
};

namespace NrvArrowSwitchMulti {
    NERVE(ArrowSwitchMultiNrvWait);
    NERVE(ArrowSwitchMultiNrvRotate);
};