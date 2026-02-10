#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TombSpiderThreadAttacher : public LiveActor {
public:
    TombSpiderThreadAttacher(LiveActor*, const char*, f32, f32);

    virtual void init(const JMapInfoIter& rIter);

    void exeFree();
    void exeAttach();

    /* 0x8C */ MtxPtr mJointMtx;
    /* 0x90 */ f32 mRadius;
    /* 0x94 */ f32 mZOffset;
};
