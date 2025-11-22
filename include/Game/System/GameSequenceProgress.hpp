#pragma once

#include "Game/System/NerveExecutor.hpp"

class FindingLuigiEventScheduler;
class GalaxyCometScheduler;
class GalaxyMoveArgument;
class LuigiLeftSupplier;
class PlayerMissLeft;
class StarPointerOnOffController;
class StorySequenceExecutor;

class GameSequenceProgress : public NerveExecutor {
public:
    /// @brief Creates a new `GameSequenceProgress`.
    GameSequenceProgress();

    void initAfterResourceLoaded();
    void update();
    void draw();
    void startScene();
    void endScene();
    void requestChangeScene(const char*);
    void requestGalaxyMove(const GalaxyMoveArgument&);
    void requestCancelScenarioSelect();
    GalaxyCometScheduler* getGalaxyCometScheduler();
    void exeBooting();
    void exeLogo();
    void exeWaitGoToFirstScene();
    void exeNormal();
    void exeGalaxyMove();
    void exeResetProcessing();
    static bool isScenePermittedIsUpdateWiiRemoteStatus() NO_INLINE;
    static bool isSceneLongAutoSleepWiiRemote();
    void updateGameDataBeforeChangeScene();
    void resetGameDataAfterChangeScene(const GalaxyMoveArgument&);
    void updateGameDataGalaxyVisitedFlag();
    void setMinFrameBeforeStartNextStage(const GalaxyMoveArgument&);
    bool isLuigiLeftSupply();
    void offLuigiLeftSupply();
    void countDownGameEventValueFromNewPowerStar();

    /* 0x08 */ StarPointerOnOffController* mStarPointerOnOffController;
    /* 0x0C */ StorySequenceExecutor* mStorySequenceExecutor;
    /* 0x10 */ FindingLuigiEventScheduler* mFindingLuigiEventScheduler;
    /* 0x14 */ GalaxyCometScheduler* mGalaxyCometScheduler;
    /* 0x18 */ LuigiLeftSupplier* mLuigiLeftSupplier;
    /* 0x1C */ PlayerMissLeft* mPlayerMissLeft;
    /* 0x20 */ s32 _20;
    /* 0x24 */ bool _24;
    /* 0x25 */ bool _25;
    /* 0x26 */ bool _26;
};
