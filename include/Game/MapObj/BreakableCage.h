#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ModelObj.h"
#include "Game/MapObj/DummyDisplayModel.h"

namespace {
    Vec cHitSensorOffsetCage;
    Vec cHitSensorOffsetFixation;
};

class BreakableCage : public LiveActor {
public:
    BreakableCage(const char *);

    enum CageType {
        CAGE_INVALID    = 0,
        CAGE_NORMAL     = 1,
        CAGE_LARGE      = 2,
        CAGE_FIX        = 3,
        CAGE_TRASH      = 4
    };

    virtual ~BreakableCage();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    const ActorCameraInfo* getCamInfo() const {
        return mCameraInfo;
    }

    void initMapToolInfo(const JMapInfoIter &);
    void initModel(const char *, const JMapInfoIter &);
    void initBaseMtxForCage();
    bool isTypeCage() const;
    bool isAppearPowerStar() const;
    bool tryBreak();
    void exeWait();
    void exeBreak();

    TPos3f mMtx;                                // _8C
    ModelObj* mBreakModel;                      // _BC
    CageType mCageType;                         // _C0
    f32 mRotationSpeed;                         // _C4
    DummyDisplayModel* mItemModel;              // _C8
    bool mIgnoreGravity;                        // _CC
    bool mDelayDeadActivate;                    // _CD
    const ActorCameraInfo* mCameraInfo;      // _D0
};

namespace NrvBreakableCage {
    NERVE(BreakableCageNrvWait);
    NERVE(BreakableCageNrvWaitStartDemoBreak);
    NERVE(BreakableCageNrvBreak);
};