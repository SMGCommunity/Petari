#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameDataConst.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/Util.hpp"

GalaxyStatusAccessor::GalaxyStatusAccessor(const ScenarioData *pScenarioData) {
    mScenarioData = pScenarioData;
}

const char* GalaxyStatusAccessor::getName() const {
    return mScenarioData->mGalaxyName;
}

u32 GalaxyStatusAccessor::getScenarioNum() const {
    return mScenarioData->getScenarioNum();
}

u32 GalaxyStatusAccessor::getPowerStarNum() const  {
    return mScenarioData->getPowerStarNum();
}

u32 GalaxyStatusAccessor::getZoneNum() const  {
    return mScenarioData->getZoneNum();
}

u32 GalaxyStatusAccessor::getZoneId(const char *pZone) const {
    return mScenarioData->getZoneId(pZone);
}

const char* GalaxyStatusAccessor::getZoneName(int zoneId) const {
    return mScenarioData->getZoneName(zoneId);
}

const char* GalaxyStatusAccessor::getCometName(s32 scenario) const {
    const char *pCometName = nullptr;

    if (mScenarioData->getValueString("Comet", scenario, &pCometName)) {
        return pCometName;
    }
    else {
        return nullptr;
    }
}

bool GalaxyStatusAccessor::isValidNormalComet(s32 scenario) const {
    bool ret = false;

    if (isExistAnyComet() && scenario == getNormalCometScenarioNo()) {
        ret = true;
    }

    return ret;
}

bool GalaxyStatusAccessor::isCometStar(s32 scenario) const {
    bool ret = false;

    if (isExistAnyComet() && scenario == getNormalCometScenarioNo() || isValidCoin100(scenario)) {
        ret = true;
    }

    return ret;
}

bool GalaxyStatusAccessor::isExistGrandStar() const {
    for (s32 i = 1; i <= mScenarioData->getPowerStarNum(); i++) {
        if (GameDataConst::isGrandStar(mScenarioData->mGalaxyName, i)) {
            return true;
        }
    }

    return false;
}

#ifdef NON_MATCHING // The good old registers problem
bool GalaxyStatusAccessor::isExistAnyComet() const {
    const char *pIsComet;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++) {
        const char *pComet;

        if (mScenarioData->getValueString("Comet", i, &pComet)) {
            pIsComet = pComet;
        }
        else {
            pIsComet = nullptr;
        }

        if (pIsComet) {
            return true;
        }
    }

    return false;
}
#endif 

s32 GalaxyStatusAccessor::getNormalCometScenarioNo() const {
    const char *pNormal;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++) {
        const char *pIsNormal = nullptr;

        if (mScenarioData->getValueString("Comet", i, &pIsNormal)) {
            pNormal = pIsNormal;
        }
        else {
            pNormal = nullptr;
        }

        if (pNormal && !isValidCoin100(i)) {
            return i;   
        }
    }

    return -1;
}

s32 GalaxyStatusAccessor::getCoin100CometScenarioNo() const {
    const char *pPurpleComet;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++) {
        const char *pCometName = nullptr;

        if (mScenarioData->getValueString("Comet", i, &pCometName)) {
            pPurpleComet = pCometName;
        }
        else {
            pPurpleComet = nullptr;
        }

        if (pPurpleComet && isValidCoin100(i)) {
            return i;   
        }
    }

    return -1;
}

bool GalaxyStatusAccessor::canOpen() const {
    if (mScenarioData->getPowerStarNum()) {
        return true;
    }
    else {
        return GameDataFunction::canOnGameEventFlag(mScenarioData->mGalaxyName);
    }
}

bool GalaxyStatusAccessor::isOpened() const {
    if (mScenarioData->getPowerStarNum() == 0) {
        return true;
    }
    else {
        return GameDataFunction::isOnGameEventFlag(mScenarioData->mGalaxyName);
    }
}

bool GalaxyStatusAccessor::isCompleted() const {
    if (mScenarioData->getPowerStarNum() == 0) {
        return false;
    }
    
    return getPowerStarNumOwned() == mScenarioData->getPowerStarNum();
}

s32 GalaxyStatusAccessor::getPowerStarNumOwned() const {
    if (!mScenarioData->getPowerStarNum()) {
        return GameDataFunction::getPowerStarNumOwned(mScenarioData->mGalaxyName);
    }
    else {
        return 0;
    }
}

bool GalaxyStatusAccessor::hasPowerStar(s32 Scenario) const {
    if (mScenarioData->getPowerStarNum() == 0) {
        return false;
    }
    else {
        return GameDataFunction::hasPowerStar(mScenarioData->mGalaxyName, Scenario);
    }
}

s32 GalaxyStatusAccessor::getNormalScenarioNum() const {
    const char *pCometName;
    s32 scenarioNum = 0;

    for (s32 i = 0; i < mScenarioData->getScenarioNum(); i++) {
        const char *pScenarioComet = nullptr;

        if (mScenarioData->getValueString("Comet", i + 1, &pScenarioComet)) {
            pCometName = pScenarioComet;
        }
        else {
            pCometName = nullptr;
        }
        
        if (!pCometName) {
            scenarioNum++;
        }
    }

    return scenarioNum;
}

const char* GalaxyStatusAccessor::getAppearPowerStarObjName(s32 scenario) const {
    const char *pObjName;
    bool ret = mScenarioData->getValueString("AppearPowerStarObj", scenario, &pObjName);

    if (isValidCoin100(scenario)){
        return "１００枚コイン";
    }

    if (ret){
        return pObjName;
    }

    return 0;
}

u32 GalaxyStatusAccessor::getActivePowerStarId(s32 scenario) const {
    u32 powerStarId;

    if (mScenarioData->getValueU32("PowerStarId", scenario, &powerStarId)) {
        return powerStarId;
    }
    else {
        return 0;
    }
}

bool GalaxyStatusAccessor::isValidCoin100(s32 scenario) const {
    const char *pPurpleComet;
    const char *pCometName = nullptr;

    if (mScenarioData->getValueString("Comet", scenario, &pCometName)) {
        pPurpleComet = pCometName;
    }
    else {
        pPurpleComet = nullptr;
    }

    return (pPurpleComet && MR::isEqualString(pPurpleComet, "Purple") || MR::isEqualString(pPurpleComet, "Black"));
}

bool GalaxyStatusAccessor::isHiddenStar(s32 scenario) const {
    bool isHidden = false;
    mScenarioData->getValueBool("IsHidden", scenario, &isHidden);
    return isHidden;
}

namespace MR {
    GalaxyStatusAccessor makeGalaxyStatusAccessor(const char *pGalaxyName) {
        return ScenarioDataFunction::getScenarioDataParser().makeAccessor(pGalaxyName);
    }

    GalaxyStatusAccessor makeCurrentGalaxyStatusAccessor() {
        return ScenarioDataFunction::getScenarioDataParser().makeAccessor(MR::getCurrentStageName());
    }
};