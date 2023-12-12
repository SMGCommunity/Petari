#pragma once

#include "Game/LiveActor/LiveActor.h"

class AirBubbleGenerator : public LiveActor {
public:
    AirBubbleGenerator(const char *);

    virtual ~AirBubbleGenerator();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void exeWait();
    void exeGenerate();

    u32 _8C;
    s32 mSpawnDelay;        // _90
    s32 mBubbleDuration;    // _94
};

namespace NrvAirBubbleGenerator {
    NERVE_DECL(AirBubbleGeneratorNrvGenerate, AirBubbleGenerator, AirBubbleGenerator::exeGenerate);
    NERVE_DECL(AirBubbleGenerateNrvWait, AirBubbleGenerator, AirBubbleGenerator::exeWait);
};