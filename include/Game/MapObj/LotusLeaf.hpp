#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class LotusLeaf : public LiveActor {
public:
    LotusLeaf(const char*);

    virtual void init(const JMapInfoIter&);

    void convergeToInitPos();
    void exeWait();
    void exeWaitPlayerOn();
    void exeShake();

    /* 0x8C */ TVec3f mInitPos;
    /* 0x98 */ f32 mShakeSpeed;
    /* 0x9C */ f32 mShakePeriod;
};
