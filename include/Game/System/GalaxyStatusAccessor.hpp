#pragma once

#include <revolution.h>

class ScenarioData;

class GalaxyStatusAccessor {
public:
	GalaxyStatusAccessor(const ScenarioData *);
	
    const char* getName() const;
    u32 getScenarioNum() const;
    u32 getPowerStarNum() const;
    u32 getZoneNum() const;
    u32 getZoneId(const char *) const;
    const char* getZoneName(int) const;
    const char* getCometName(s32) const;
	bool isValidNormalComet(s32) const;
	bool isCometStar(s32) const;
	bool isExistGrandStar() const;
	bool isExistAnyComet() const;
	s32 getNormalCometScenarioNo() const;
	s32 getCoin100CometScenarioNo() const;
	bool canOpen() const;
	bool isOpened() const;
	bool hasPowerStar(s32) const;
	bool isCompleted() const;
	s32 getPowerStarNumOwned() const;
	s32 getNormalScenarioNum() const;
	const char* getAppearPowerStarObjName(s32) const;
	u32 getActivePowerStarId(s32) const;
	bool isValidCoin100(s32) const;
	bool isHiddenStar(s32) const;

    const ScenarioData* mScenarioData; // 0x0
};

namespace MR {
    GalaxyStatusAccessor makeGalaxyStatusAcessor(const char *);
    GalaxyStatusAccessor makeCurrentGalaxyStatusAccessor();
};