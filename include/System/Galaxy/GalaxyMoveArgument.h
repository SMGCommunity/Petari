#pragma once

#include "MR/JMapIdInfo.h"

class GalaxyMoveArgument
{
public:
    GalaxyMoveArgument(u32, const char *, s32, const JMapIdInfo *);

    bool isEqualStage(const char *) const;
    bool isEqualStageScenario(const char *, s32) const;

    u32 _0;
    const char* mStageName; // _4
    s32 mScenarioNo; // _8
    s32 _C;
    JMapIdInfo mStartInfo; // _10
};