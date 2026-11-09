#pragma once

#include <revolution/types.h>

class GameDataHolder;

struct GalaxyScenarioInfo {
    /* 0x0 */ const char* mStageName;
    /* 0x4 */ s32 mScenarioNo;
};

class ScenarioProgressTestRun {
public:
    ScenarioProgressTestRun(GameDataHolder* pGameDataHolder);

    bool tryTakeNextPowerStar(GalaxyScenarioInfo* pInfo, u32 preference);
    bool tryTakePowerStarsToPlayGalaxyScenario(const GalaxyScenarioInfo* pInfo, bool);
    bool tryOnGameEventFlagsDependedAnother(const GalaxyScenarioInfo* pInfo);
    bool canTakePowerStar(const GalaxyScenarioInfo* pInfo) const;
    bool tryTakeNextPowerStarAndOnEventFlag(u32 preference);
    bool tryTakePowerStarsToOpenGalaxy(u32 preference, const char* pStageName, bool ignoreTargetGalaxyFlag);
    bool followGameEventFlagGalaxyOpenAndStoryProgress(const GalaxyScenarioInfo* pInfo);
    bool tryTakeNextPowerStarPreferGalaxy(GalaxyScenarioInfo* pInfo);
    bool tryTakeNextPowerStarPreferScenario(GalaxyScenarioInfo* pInfo);

private:
    /* 0x0 */ GameDataHolder* mGameDataHolder;
};
