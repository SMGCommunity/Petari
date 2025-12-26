#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataConst.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

GalaxyStatusAccessor::GalaxyStatusAccessor(const ScenarioData* pScenarioData) : mScenarioData(pScenarioData) {}

const char* GalaxyStatusAccessor::getName() const {
    return mScenarioData->mGalaxyName;
}

s32 GalaxyStatusAccessor::getScenarioNum() const {
    return mScenarioData->getScenarioNum();
}

s32 GalaxyStatusAccessor::getPowerStarNum() const {
    return mScenarioData->getPowerStarNum();
}

s32 GalaxyStatusAccessor::getZoneNum() const {
    return mScenarioData->getZoneNum();
}

s32 GalaxyStatusAccessor::getZoneId(const char* pZone) const {
    return mScenarioData->getZoneId(pZone);
}

const char* GalaxyStatusAccessor::getZoneName(int zoneId) const {
    return mScenarioData->getZoneName(zoneId);
}

const char* GalaxyStatusAccessor::getCometName(s32 scenarioNo) const {
    const char* pCometName = nullptr;

    if (mScenarioData->getValueString("Comet", scenarioNo, &pCometName)) {
        return pCometName;
    } else {
        return nullptr;
    }
}

bool GalaxyStatusAccessor::isValidNormalComet(s32 scenarioNo) const {
    return isExistAnyComet() && scenarioNo == getNormalCometScenarioNo();
}

bool GalaxyStatusAccessor::isCometStar(s32 scenarioNo) const {
    return isExistAnyComet() && scenarioNo == getNormalCometScenarioNo() || isValidCoin100(scenarioNo);
}

bool GalaxyStatusAccessor::isExistGrandStar() const {
    for (s32 i = 1; i <= mScenarioData->getPowerStarNum(); i++) {
        if (GameDataConst::isGrandStar(mScenarioData->mGalaxyName, i)) {
            return true;
        }
    }

    return false;
}

#ifdef NON_MATCHING  // The good old registers problem
bool GalaxyStatusAccessor::isExistAnyComet() const {
    const char* pIsComet;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++) {
        const char* pComet;

        if (mScenarioData->getValueString("Comet", i, &pComet)) {
            pIsComet = pComet;
        } else {
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
    const char* pNormal;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++) {
        const char* pIsNormal = nullptr;

        if (mScenarioData->getValueString("Comet", i, &pIsNormal)) {
            pNormal = pIsNormal;
        } else {
            pNormal = nullptr;
        }

        if (pNormal && !isValidCoin100(i)) {
            return i;
        }
    }

    return -1;
}

s32 GalaxyStatusAccessor::getCoin100CometScenarioNo() const {
    const char* pPurpleComet;

    for (s32 i = 1; i <= mScenarioData->getScenarioNum(); i++) {
        const char* pCometName = nullptr;

        if (mScenarioData->getValueString("Comet", i, &pCometName)) {
            pPurpleComet = pCometName;
        } else {
            pPurpleComet = nullptr;
        }

        if (pPurpleComet && isValidCoin100(i)) {
            return i;
        }
    }

    return -1;
}

bool GalaxyStatusAccessor::canOpen() const {
    if (mScenarioData->getPowerStarNum() == 0) {
        return true;
    }

    return GameDataFunction::canOnGameEventFlag(mScenarioData->mGalaxyName);
}

bool GalaxyStatusAccessor::isOpened() const {
    if (mScenarioData->getPowerStarNum() == 0) {
        return true;
    }

    return GameDataFunction::isOnGameEventFlag(mScenarioData->mGalaxyName);
}

bool GalaxyStatusAccessor::isCompleted() const {
    if (mScenarioData->getPowerStarNum() == 0) {
        return false;
    }

    return getPowerStarNumOwned() == mScenarioData->getPowerStarNum();
}

s32 GalaxyStatusAccessor::getPowerStarNumOwned() const {
    if (mScenarioData->getPowerStarNum() == 0) {
        return 0;
    }

    return GameDataFunction::getPowerStarNumOwned(mScenarioData->mGalaxyName);
}

bool GalaxyStatusAccessor::hasPowerStar(s32 scenarioNo) const {
    if (mScenarioData->getPowerStarNum() == 0) {
        return false;
    }

    return GameDataFunction::hasPowerStar(mScenarioData->mGalaxyName, scenarioNo);
}

s32 GalaxyStatusAccessor::getNormalScenarioNum() const {
    const char* pCometName;
    s32 scenarioNum = 0;

    for (s32 i = 0; i < mScenarioData->getScenarioNum(); i++) {
        const char* pScenarioComet = nullptr;

        if (mScenarioData->getValueString("Comet", i + 1, &pScenarioComet)) {
            pCometName = pScenarioComet;
        } else {
            pCometName = nullptr;
        }

        if (!pCometName) {
            scenarioNum++;
        }
    }

    return scenarioNum;
}

const char* GalaxyStatusAccessor::getAppearPowerStarObjName(s32 scenarioNo) const {
    const char* pObjName;
    bool ret = mScenarioData->getValueString("AppearPowerStarObj", scenarioNo, &pObjName);

    if (isValidCoin100(scenarioNo)) {
        return "１００枚コイン";
    }

    if (ret) {
        return pObjName;
    }

    return 0;
}

u32 GalaxyStatusAccessor::getActivePowerStarId(s32 scenarioNo) const {
    u32 powerStarId;

    if (mScenarioData->getValueU32("PowerStarId", scenarioNo, &powerStarId)) {
        return powerStarId;
    } else {
        return 0;
    }
}

bool GalaxyStatusAccessor::isValidCoin100(s32 scenarioNo) const {
    const char* pPurpleComet;
    const char* pCometName = nullptr;

    if (mScenarioData->getValueString("Comet", scenarioNo, &pCometName)) {
        pPurpleComet = pCometName;
    } else {
        pPurpleComet = nullptr;
    }

    return (pPurpleComet && MR::isEqualString(pPurpleComet, "Purple") || MR::isEqualString(pPurpleComet, "Black"));
}

bool GalaxyStatusAccessor::isHiddenStar(s32 scenarioNo) const {
    bool isHidden = false;

    mScenarioData->getValueBool("IsHidden", scenarioNo, &isHidden);

    return isHidden;
}

namespace MR {
    GalaxyStatusAccessor makeGalaxyStatusAccessor(const char* pGalaxyName) {
        return ScenarioDataFunction::getScenarioDataParser()->makeAccessor(pGalaxyName);
    }

    GalaxyStatusAccessor makeCurrentGalaxyStatusAccessor() {
        return ScenarioDataFunction::getScenarioDataParser()->makeAccessor(MR::getCurrentStageName());
    }
};  // namespace MR
