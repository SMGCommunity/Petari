#include "System/Galaxy/GalaxyStatusAccessor.h"
#include "System/Game/GameDataFunction.h"
#include "System/Game/GameDataConst.h"
#include "Util/SceneUtil.h"
#include "Util/StringUtil.h"

#include "System/ScenarioDataParser.h"

GalaxyStatusAccessor::GalaxyStatusAccessor(ScenarioData *pScenarioData)
{
    mScenarioData = pScenarioData;
}

u32 GalaxyStatusAccessor::getScenarioNum() const
{
    return mScenarioData->getScenarioNum();
}

u32 GalaxyStatusAccessor::getPowerStarNum() const 
{
    return mScenarioData->getPowerStarNum();
}

u32 GalaxyStatusAccessor::getZoneNum() const 
{
    return mScenarioData->getZoneNum();
}

u32 GalaxyStatusAccessor::getZoneId(const char *pZone) const
{
    return mScenarioData->getZoneId(pZone);
}

const char* GalaxyStatusAccessor::getZoneName(s32 zoneId) const
{
    return mScenarioData->getZoneName(zoneId);
}

const char* GalaxyStatusAccessor::getCometName(s32 scenario) const
{
    const char *pCometName = NULL;

    if (mScenarioData->getValueString("Comet", scenario, &pCometName))
    {
        return pCometName;
    }
    else
    {
        return NULL;
    }
}

bool GalaxyStatusAccessor::isValidNormalComet(s32 scenario) const
{
    bool ret = false;

    if (isExistAnyComet() && scenario == getNormalCometScenarioNo())
    {
        ret = true;
    }

    return ret;
}

bool GalaxyStatusAccessor::isCometStar(s32 scenario) const
{
    bool ret = false;

    if (isExistAnyComet() && scenario == getNormalCometScenarioNo() || isValidCoin100(scenario))
    {
        ret = true;
    }

    return ret;
}

bool GalaxyStatusAccessor::isExistGrandStar() const
{
    for (s32 i = 1; i <= mScenarioData->getPowerStarNum(); i++)
    {
        if (GameDataConst::isGrandStar(mScenarioData->mGalaxyName, i))
        {
            return true;
        }
    }

    return false;
}

bool GalaxyStatusAccessor::isExistAnyComet() const
{
    const char *pIsComet;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++)
    {
        const char *pComet;

        if (mScenarioData->getValueString("Comet", i, &pComet))
        {
            pIsComet = pComet;
        }
        else
        {
            pIsComet = NULL;
        }

        if (pIsComet)
        {
            return true;
        }
    }

    return false;
}

s32 GalaxyStatusAccessor::getNormalCometScenarioNo() const
{
    const char *pNormal;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++)
    {
        const char *pIsNormal = NULL;

        if (mScenarioData->getValueString("Comet", i, &pIsNormal))
        {
            pNormal = pIsNormal;
        }
        else
        {
            pNormal = NULL;
        }

        if (pNormal && !isValidCoin100(i))
        {
            return i;   
        }
    }

    return -1;
}

s32 GalaxyStatusAccessor::getCoin100CometScenarioNo() const
{
    const char *pPurpleComet;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++)
    {
        const char *pCometName = NULL;

        if (mScenarioData->getValueString("Comet", i, &pCometName))
        {
            pPurpleComet = pCometName;
        }
        else
        {
            pPurpleComet = NULL;
        }

        if (pPurpleComet && isValidCoin100(i))
        {
            return i;   
        }
    }

    return -1;
}

bool GalaxyStatusAccessor::canOpen() const
{
    if (mScenarioData->getPowerStarNum())
    {
        return GameDataFunction::canOnGameEventFlag(mScenarioData->mGalaxyName);
    }
    else
    {
        return true;
    }
}

bool GalaxyStatusAccessor::isOpened() const
{
    if (mScenarioData->getPowerStarNum() == 0)
    {
        return true;
    }
    else
    {
        return GameDataFunction::isOnGameEventFlag(mScenarioData->mGalaxyName);
    }
}

bool GalaxyStatusAccessor::hasPowerStar(s32 Scenario) const
{
    if (mScenarioData->getPowerStarNum() == 0)
    {
        return false;
    }
    else
    {
        return GameDataFunction::hasPowerStar(mScenarioData->mGalaxyName, Scenario);
    }
}

bool GalaxyStatusAccessor::isCompleted() const
{
    if (mScenarioData->getPowerStarNum() == 0)
    {
        return false;
    }
    
    return getPowerStarNumOwned() == mScenarioData->getPowerStarNum();
}

s32 GalaxyStatusAccessor::getPowerStarNumOwned() const
{
    if (!mScenarioData->getPowerStarNum())
    {
        return GameDataFunction::getPowerStarNumOwned(mScenarioData->mGalaxyName);
    }
    else
    {
        return 0;
    }
}

s32 GalaxyStatusAccessor::getNormalScenarioNum() const
{
    const char *pCometName;
    s32 scenarioNum = 0;

    for (s32 i = 0; i < mScenarioData->getScenarioNum(); i++)
    {
        const char *pScenarioComet = NULL;

        if (mScenarioData->getValueString("Comet", i + 1, &pScenarioComet))
        {
            pCometName = pScenarioComet;
        }
        else    
        {
            pCometName = NULL;
        }
        
        if (!pCometName)
        {
            scenarioNum++;
        }
    }

    return scenarioNum;
}

const char* GalaxyStatusAccessor::getAppearPowerStarObjName(s32 scenario) const
{
    const char *pObjName;
    bool ret = mScenarioData->getValueString("AppearPowerStarObj", scenario, &pObjName);

    if (isValidCoin100(scenario))
    {
        return "１００枚コイン";
    }

    if (ret)
    {
        return pObjName;
    }

    return 0;
}

u32 GalaxyStatusAccessor::getActivePowerStarId(s32 scenario) const
{
    u32 powerStarId;

    if (mScenarioData->getValueU32("PowerStarId", scenario, &powerStarId))
    {
        return powerStarId;
    }
    else
    {
        return 0;
    }
}

bool GalaxyStatusAccessor::isValidCoin100(s32 scenario) const
{
    const char *pPurpleComet;
    const char *pCometName = NULL;

    if (mScenarioData->getValueString("Comet", scenario, &pCometName))
    {
        pPurpleComet = pCometName;
    }
    else
    {
        pPurpleComet = NULL;
    }

    return (pPurpleComet && MR::isEqualString(pPurpleComet, "Purple") || MR::isEqualString(pPurpleComet, "Black"));
}

bool GalaxyStatusAccessor::isHiddenStar(s32 scenario) const
{
    bool isHidden = false;
    mScenarioData->getValueBool("IsHidden", scenario, &isHidden);

    return isHidden;
}

namespace MR
{
    GalaxyStatusAccessor* makeGalaxyStatusAccessor(const char *pGalaxy)
    {
        ScenarioDataParser* pScenarioDataParser = ScenarioDataFunction::getScenarioDataParser();
        return pScenarioDataParser->makeAccessor(pGalaxy);
    }

    GalaxyStatusAccessor* makeCurrentGalaxyStatusAccessor()
    {
        const char *pGalaxy = MR::getCurrentStageName();
        ScenarioDataParser* pScenarioDataParser = ScenarioDataFunction::getScenarioDataParser();
        return pScenarioDataParser->makeAccessor(pGalaxy);
    }
};