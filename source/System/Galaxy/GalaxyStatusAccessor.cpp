#include "System/Galaxy/GalaxyStatusAccessor.h"
#include "System/Game/GameDataFunction.h"
#include "System/Game/GameDataConst.h"
#include "Util/SceneUtil.h"
#include "Util/StringUtil.h"

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
    return mScenarioData->getPowerStarNum();
}

u32 GalaxyStatusAccessor::getZoneId(const char *pZone) const
{
    return mScenarioData->getZoneId(pZone);
}

const char* GalaxyStatusAccessor::getZoneName(s32 Id) const
{
    return mScenarioData->getZoneName(Id);
}

const char* GalaxyStatusAccessor::getCometName(s32 Scenario) const
{
    const char *pCometName = NULL;

    if (mScenarioData->getValueString("Comet", Scenario, &pCometName))
    {
        return pCometName;
    }
    else
    {
        return NULL;
    }
}

bool GalaxyStatusAccessor::isValidNormalComet(s32 Scenario) const
{
    bool ret = false;

    if (isExistAnyComet() && Scenario == getNormalCometScenarioNo())
    {
        ret = true;
    }

    return ret;
}

bool GalaxyStatusAccessor::isCometStar(s32 Scenario) const
{
    bool ret = false;

    if (isExistAnyComet() && Scenario == getNormalCometScenarioNo() || isValidCoin100(Scenario))
    {
        ret = true;
    }

    return ret;
}

bool GalaxyStatusAccessor::isExistGrandStar() const
{
    for (s32 i = 1; i <= mScenarioData->getPowerStarNum(); i++)
    {
        if (GameDataConst::isGrandStar(mScenarioData->_4, i))
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
        const char *_8 = NULL;

        if (mScenarioData->getValueString("Comet", i, &_8))
        {
            pNormal = _8;
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
        return GameDataFunction::canOnGameEventFlag(mScenarioData->_4);
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
        return GameDataFunction::isOnGameEventFlag(mScenarioData->_4);
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
        return GameDataFunction::hasPowerStar(mScenarioData->_4, Scenario);
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
        return GameDataFunction::getPowerStarNumOwned(mScenarioData->_4);
    else
        return 0;
}

s32 GalaxyStatusAccessor::getNormalScenarioNum() const
{
    const char *cometName;
    s32 scenarioNum = 0;

    for (s32 i = 0; i < mScenarioData->getScenarioNum(); i++)
    {
        const char *scenarioComet = NULL;

        if (mScenarioData->getValueString("Comet", i + 1, &scenarioComet))
        {
            cometName = scenarioComet;
        }
        else    
        {
            cometName = NULL;
        }
        
        if (!cometName)
        {
            scenarioNum++;
        }
    }

    return scenarioNum;
}

const char* GalaxyStatusAccessor::getAppearPowerStarObjName(s32 scenario) const
{
    const char *pObjName;
    const char *pAppearPowerStarObj = mScenarioData->getValueString("AppearPowerStarObj", scenario, &pObjName);

    if (isValidCoin100(scenario))
    {
        return "１００枚コイン";
    }

    if (pAppearPowerStarObj)
    {
        return pObjName;
    }

    return 0;
}

u32 GalaxyStatusAccessor::getActivePowerStarId(s32 Scenario) const
{
    u32 powerStarId;

    if (mScenarioData->getValueU32("PowerStarId", Scenario, &powerStarId))
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
    // very close 
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

    bool ret = false;

    if (pPurpleComet && MR::isEqualString(pPurpleComet, "Purple") || MR::isEqualString(pPurpleComet, "Black"))
    {
        ret = true;
    }

    return ret;
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
        ScenarioDataParser* parser = ScenarioDataFunction::getScenarioDataParser();
        return parser->makeAccessor(pGalaxy);
    }

    GalaxyStatusAccessor* makeCurrentGalaxyStatusAccessor()
    {
        const char *Galaxy = MR::getCurrentStageName();
        ScenarioDataParser* parser = ScenarioDataFunction::getScenarioDataParser();
        return parser->makeAccessor(Galaxy);
    }
};