#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/MapObj/DummyDisplayModel.h"

class CrystalCage : public LiveActor {
public:
    CrystalCage(const char *);

    virtual ~CrystalCage();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    void forceBreak();
    void initMapToolInfo(const JMapInfoIter &);
    void initModel(const char *);
    void tryOnSwitchDead();
    void exeWait();
    void exeBreak();
    void exeBreakAfter();

    s32 mCrystalCageType;                           // _8C
    ModelObj* mBreakObj;                            // _90
    TPos3f _94;
    s32 _C4;
    s32 _C8;
    RumbleCalculatorCosMultLinear* mRumbleCalc;     // _CC
    TVec3f _D0;
    TVec3f _DC;
    TVec3f _E8;
    DummyDisplayModel* mDisplayModel;               // _F4
    TVec3f _F8;
    bool _104;
    u8 _105;
    u8 _106;
    u8 _107;
    s32 _108;
    bool mIsBreakObjVisible;                        // _10C
    bool mPlayRiddleSFX;                            // _10D
    bool mHasBinding;                               // _10E
    u8 _10F;
    TVec3f _110;
};

namespace NrvCrystalCage {
    NERVE(CrystalCageNrvWait);
    NERVE(CrystalCageNrvBreak);
    NERVE(CrystalCageNrvBreakAfter);
};
