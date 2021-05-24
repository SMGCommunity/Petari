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
    const char *cometName = NULL;

    if (mScenarioData->getValueString("Comet", Scenario, &cometName))
        return cometName;
    else
        return NULL;
}

bool GalaxyStatusAccessor::isValidNormalComet(s32 Scenario) const
{
    bool ret = false;

    if (isExistAnyComet() && Scenario == getNormalCometScenarioNo())
        ret = true;
    return ret;
}

bool GalaxyStatusAccessor::isCometStar(s32 Scenario) const
{
    bool ret = false;

    if (isExistAnyComet() && Scenario == getNormalCometScenarioNo() || isValidCoin100(Scenario))
        ret = true;
    return ret;
}

bool GalaxyStatusAccessor::isExistGrandStar() const
{
    for (s32 i = 1; i <= mScenarioData->getPowerStarNum(); i++)
    {
        if (GameDataConst::isGrandStar(mScenarioData->_4, i))
            return true;
    }
    return false;
}

bool GalaxyStatusAccessor::isExistAnyComet() const
{
    const char *isComet;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++)
    {
        const char *comet;

        if (mScenarioData->getValueString("Comet", i, &comet))
            isComet = comet;
        else
            isComet = NULL;

        if (isComet)
            return true;
    }
    return false;
}

s32 GalaxyStatusAccessor::getNormalCometScenarioNo() const
{
    const char *normal;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++)
    {
        const char *_8 = NULL;

        if (mScenarioData->getValueString("Comet", i, &_8))
            normal = _8;
        else
            normal = NULL;

        if (normal && !isValidCoin100(i))
            return i;   
    }
    return -1;
}

s32 GalaxyStatusAccessor::getCoin100CometScenarioNo() const
{
    const char *purple;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++)
    {
        const char *cometName = NULL;

        if (mScenarioData->getValueString("Comet", i, &cometName))
            purple = cometName;
        else
            purple = NULL;

        if (purple && isValidCoin100(i))
            return i;   
    }
    return -1;
}

bool GalaxyStatusAccessor::canOpen() const
{
    if (mScenarioData->getPowerStarNum())
        return GameDataFunction::canOnGameEventFlag(mScenarioData->_4);
    else
        return true;
}

bool GalaxyStatusAccessor::isOpened() const
{
    if (mScenarioData->getPowerStarNum() == 0)
        return true;
    else
        return GameDataFunction::isOnGameEventFlag(mScenarioData->_4);
}

bool GalaxyStatusAccessor::hasPowerStar(s32 Scenario) const
{
    if (mScenarioData->getPowerStarNum() == 0)
        return false;
    else
        return GameDataFunction::hasPowerStar(mScenarioData->_4, Scenario);;
}

bool GalaxyStatusAccessor::isCompleted() const
{
    if (mScenarioData->getPowerStarNum() == 0)
        return false;
    
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
    s32 ScenarioNum = 0;

    for (s32 i = 0; i < mScenarioData->getScenarioNum(); i++)
    {
        const char *_8 = NULL;
        if (mScenarioData->getValueString("Comet", i + 1, &_8))
            cometName = _8;
        else    
            cometName = NULL;
        
        if (!cometName)
            ScenarioNum++;
    }
    return ScenarioNum;
}

const char* GalaxyStatusAccessor::getAppearPowerStarObjName(s32 Scenario) const
{
    const char *ObjName;
    const char *AppearPowerStarObj = mScenarioData->getValueString("AppearPowerStarObj", Scenario, &ObjName);

    if (isValidCoin100(Scenario))
        return "１００枚コイン";

    if (AppearPowerStarObj)
        return ObjName;

    return 0;
}

u32 GalaxyStatusAccessor::getActivePowerStarId(s32 Scenario) const
{
    u32 Id;

    if (mScenarioData->getValueU32("PowerStarId", Scenario, &Id))
        return Id;
    else
        return 0;
}

bool GalaxyStatusAccessor::isValidCoin100(s32 Scenario) const
{
    // very close 
    const char *purple;
    const char *cometName = NULL;
    
    if (mScenarioData->getValueString("Comet", Scenario, &cometName))
        purple = cometName;
    else
        purple = NULL;

    bool ret = false;

    if (purple && MR::isEqualString(purple, "Purple") || MR::isEqualString(purple, "Black"))
        ret = true;
    return ret;
}

bool GalaxyStatusAccessor::isHiddenStar(s32 Scenaio) const
{
    bool isHidden = false;
    mScenarioData->getValueBool("IsHidden", Scenaio, &isHidden);

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