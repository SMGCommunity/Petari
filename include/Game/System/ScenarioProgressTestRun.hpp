#pragma once

#include <revolution/types.h>

class GameDataHolder;

struct GalaxyScenarioInfo {
    const char* mStageName;
    s32 mScenarioNo;
};

class ScenarioProgressTestRun {
public:
    bool tryTakeNextPowerStar(GalaxyScenarioInfo* pInfo, u32 preference);
    bool tryOnGameEventFlagsDependedAnother(const GalaxyScenarioInfo* pInfo);
    bool canTakePowerStar(const GalaxyScenarioInfo* pInfo) const;

    ScenarioProgressTestRun(GameDataHolder* pGameDataHolder);

    bool tryTakeNextPowerStarAndOnEventFlag(u32 preference);
    bool tryTakePowerStarsToOpenGalaxy(u32 preference, const char* pStageName, bool ignoreTargetGalaxyFlag);
    bool tryTakeNextPowerStarPreferGalaxy(GalaxyScenarioInfo* pInfo);
    bool tryTakeNextPowerStarPreferScenario(GalaxyScenarioInfo* pInfo);

private:
    /* 0x0 */ GameDataHolder* mGameDataHolder;
};
