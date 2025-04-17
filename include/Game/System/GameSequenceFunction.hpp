#pragma once

class GameSequenceFunction {
public:
    static void requestCancelScenarioSelect();

    static bool isNeedMoviePlayerForStorySequenceEvent();

    static bool isActiveSaveDataHandleSequence();
    static void startGameDataSaveSequence(bool, bool);
};
