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

    u32 _8C;
    s32 mSpawnDelay;      // 0x90
    s32 mBubbleDuration;  // 0x94
};
