#pragma once

#include "Game/Util/JMapIdInfo.hpp"
#include <revolution/types.h>

#define GALAXY_MOVE_MOVING 0
#define GALAXY_MOVE_SYNC_GAME_DATA 1
#define GALAXY_MOVE_SCENARIO_SELECT 2
#define GALAXY_MOVE_BACK_ASTRO_DOME 3
#define GALAXY_MOVE_AFTER_CLEAR 4
#define GALAXY_MOVE_AFTER_MISS 5
#define GALAXY_MOVE_AFTER_LOADING_GAME_DATA 6
#define GALAXY_MOVE_TITLE 7

class GalaxyMoveArgument {
public:
    GalaxyMoveArgument(u32 moveType, const char* pStageName, s32 scenarioNo, const JMapIdInfo* pMapIdInfo);

    bool isEqualStage(char const* pStageName) const;
    bool isEqualStageScenario(char const* pStageName, int scenarioNo) const;

    /* 0x00 */ u32 mMoveType;
    /* 0x04 */ const char* mStageName;
    /* 0x08 */ s32 mScenarioNo;
    /* 0x0C */ s32 mSelectedScenarioNo;
    /* 0x10 */ JMapIdInfo mIDInfo;
};
