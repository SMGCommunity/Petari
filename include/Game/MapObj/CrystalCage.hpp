#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class DummyDisplayModel;
class ModelObj;
class RumbleCalculatorCosMultLinear;

class CrystalCage : public LiveActor {
public:
    CrystalCage(const char*);

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

    /* 0x08C */ s32 mCrystalCageType;
    /* 0x090 */ ModelObj* mBreakObj;
    /* 0x094 */ TPos3f _94;
    /* 0x0C4 */ s32 _C4;
    /* 0x0C8 */ s32 _C8;
    /* 0x0CC */ RumbleCalculatorCosMultLinear* mRumbleCalc;
    /* 0x0D0 */ TVec3f _D0;
    /* 0x0DC */ TVec3f _DC;
    /* 0x0E8 */ TVec3f _E8;
    /* 0x0F4 */ DummyDisplayModel* mDisplayModel;
    /* 0x0F8 */ TVec3f _F8;
    /* 0x104 */ bool _104;
    /* 0x105 */ u8 _105;
    /* 0x106 */ u8 _106;
    /* 0x107 */ u8 _107;
    /* 0x108 */ s32 _108;
    /* 0x10C */ bool mIsBreakObjVisible;
    /* 0x10D */ bool mPlayRiddleSFX;
    /* 0x10E */ bool mHasBinding;
    /* 0x10F */ u8 _10F;
    /* 0x110 */ TVec3f _110;
};
