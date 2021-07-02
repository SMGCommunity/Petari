#pragma once

#include "smg.h"

class ScenarioData;

class GalaxyStatusAccessor
{
public:
	GalaxyStatusAccessor(ScenarioData *);
	
    u32 getScenarioNum() const;
    u32 getPowerStarNum() const;
    u32 getZoneNum() const;
    u32 getZoneId(const char *) const;
    const char* getZoneName(s32) const;
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

    ScenarioData* mScenarioData; // _0
};

namespace MR
{
    static GalaxyStatusAccessor* makeGalaxyStatusAcessor(const char *);
    static GalaxyStatusAccessor* makeCurrentGalaxyStatusAccessor();
};