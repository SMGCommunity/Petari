#pragma once

#include "Game/Util/JMapIdInfo.hpp"
#include <revolution/types.h>

class GalaxyMoveArgument {
public:
    GalaxyMoveArgument(u32, const char*, s32, const JMapIdInfo*);

    bool isEqualStage(char const*) const;
    bool isEqualStageScenario(char const*, int) const;

    /* 0x0 */ u32 mMoveType;
    /* 0x4 */ const char* mStageName;
    /* 0x8 */ s32 mScenarioNo;
    s32 _C;
    /* 0x10 */ JMapIdInfo mIDInfo;
};
