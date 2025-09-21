#pragma once

#include "Game/System/GalaxyMoveArgument.hpp"
#include "revolution/types.h"

class GameSequenceFunction {
public:
    static void requestGalaxyMove(const GalaxyMoveArgument &);
    static void requestCancelScenarioSelect();

    static bool isNeedMoviePlayerForStorySequenceEvent();

    static s32 getExecutingStorySequenceEventNum();

    static bool isActiveSaveDataHandleSequence();
    static void startGameDataSaveSequence(bool, bool);

    static void sendStageResultSequenceParam(const char *, s32, int, int);
};
