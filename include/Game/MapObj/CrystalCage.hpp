#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/DummyDisplayModel.hpp"

class CrystalCage : public LiveActor {
public:
    CrystalCage(const char*);

    virtual ~CrystalCage();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void forceBreak();
    void initMapToolInfo(const JMapInfoIter&);
    void initModel(const char*);
    void tryOnSwitchDead();
    void exeWait();
    void exeBreak();
    void exeBreakAfter();

    s32 mCrystalCageType;  // 0x8C
    ModelObj* mBreakObj;   // 0x90
    TPos3f _94;
    s32 _C4;
    s32 _C8;
    RumbleCalculatorCosMultLinear* mRumbleCalc;  // 0xCC
    TVec3f _D0;
    TVec3f _DC;
    TVec3f _E8;
    DummyDisplayModel* mDisplayModel;  // 0xF4
    TVec3f _F8;
    bool _104;
    u8 _105;
    u8 _106;
    u8 _107;
    s32 _108;
    bool mIsBreakObjVisible;  // 0x10C
    bool mPlayRiddleSFX;      // 0x10D
    bool mHasBinding;         // 0x10E
    u8 _10F;
    TVec3f _110;
};
