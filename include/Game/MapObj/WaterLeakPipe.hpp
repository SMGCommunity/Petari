#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class IceStepNoSlip : public ModelObj {
public:
    IceStepNoSlip(MtxPtr);

    virtual ~IceStepNoSlip();
    virtual void init(const JMapInfoIter&);
    virtual void appear();

    void exeAppear();
    void exeBreak();
};

class WaterLeakPipe : public LiveActor {
public:
    WaterLeakPipe(const char*);

    virtual ~WaterLeakPipe();
    virtual void init(const JMapInfoIter&);
    virtual void calcAnim(){};
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void initPipeHeight();

    void exeWait();
    void exeFreeze();

    IceStepNoSlip* mIceStep;  // 0x8C
    f32 mPipeHeight;          // 0x90
    MtxPtr mTopMtx;           // 0x94
    MtxPtr mBottomMtx;        // 0x98
    TVec3f _9C;
};
