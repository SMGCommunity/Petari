#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class DummyDisplayModel;
class ModelObj;
class RumbleCalculatorCosMultLinear;

class CrystalCage : public LiveActor {
public:
    CrystalCage(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x2C */ virtual void kill();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

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
