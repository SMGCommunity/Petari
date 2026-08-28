#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class IceStepNoSlip : public ModelObj {
public:
    IceStepNoSlip(MtxPtr);

    /* 0x08 */ virtual ~IceStepNoSlip();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();

    void exeAppear();
    void exeBreak();
};

class WaterLeakPipe : public LiveActor {
public:
    WaterLeakPipe(const char*);

    /* 0x08 */ virtual ~WaterLeakPipe();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim(){};
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void initPipeHeight();

    void exeWait();
    void exeFreeze();

    IceStepNoSlip* mIceStep;  // 0x8C
    f32 mPipeHeight;          // 0x90
    MtxPtr mTopMtx;           // 0x94
    MtxPtr mBottomMtx;        // 0x98
    TVec3f _9C;
};
