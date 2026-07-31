#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AirBubbleGenerator : public LiveActor {
public:
    AirBubbleGenerator(const char*);

    virtual ~AirBubbleGenerator();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void exeWait();
    void exeGenerate();

    /* 0x8C */ u32 _8C;
    /* 0x90 */ s32 mWaitTime;
    /* 0x94 */ s32 mBubbleLifeTime;
};
