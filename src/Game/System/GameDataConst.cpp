#include "Game/System/GameDataConst.hpp"
#include "Game/System/GalaxyInfoTable.hpp"
#include "Game/System/GameEventFlag.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/Util.hpp"
#include <cstdio>

s32 GameDataConst::getPowerStarNumToOpenGalaxy(const char* pGalaxyName) {
    JMapInfo info;
    info.attach(&cGalaxyInfoTable);

    JMapInfoIter element = info.findElement< const char* >("name", pGalaxyName, 0);

    u32 powerStarNum = 0;
    element.getValue< u32 >("PowerStarNum", &powerStarNum);

    return powerStarNum;
}

bool GameDataConst::isPowerStarGreen(const char* pGalaxyName, s32 starId) {
    return isPowerStarSpecial(pGalaxyName, starId, "SpecialStarGreen");
}

bool GameDataConst::isPowerStarRed(const char* pGalaxyName, s32 starId) {
    return isPowerStarSpecial(pGalaxyName, starId, "SpecialStarRed");
}

bool GameDataConst::isPowerStarLuigiHas(const char* pGalaxyName, s32 starId) {
    return isPowerStarSpecial(pGalaxyName, starId, "SpecialStarFindingLuigi");
}

bool GameDataConst::isGrandStar(const char* pGalaxyName, s32 starId) {
    return isPowerStarSpecial(pGalaxyName, starId, "SpecialStarGrand");
}

bool GameDataConst::isGalaxyLuigiArrested(const char* pGalaxyName, s32 starId) {
    GameEventFlagAccessor accessor = GameEventFlagTable::makeAccessor("SpecialStarLuigiRescued");

    return MR::isEqualString(accessor.getGalaxyName(), pGalaxyName) && (starId == -1 || starId == accessor.getStarId());
}

bool GameDataConst::isQuestionGalaxy(const char* pGalaxyName) {
    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor accessor(iter.getFlag());

        if (!accessor.isTypeGalaxyOpenStar()) {
            continue;
        }

        char flagName[64];
        snprintf(flagName, sizeof(flagName), "Appear%s", pGalaxyName);

        if (!MR::isEqualString(accessor.getName(), flagName)) {
            continue;
        }

        return true;
    }

    return false;
}

bool GameDataConst::isGalaxyAppearGreenDriver(const char* pGalaxyName) {
    char flagName[48];
    snprintf(flagName, sizeof(flagName), "Appear%s", pGalaxyName);

    return !GameEventFlagTable::isExist(flagName) ? false : GameEventFlagTable::isDependedAnother(flagName, "SpecialStarGreenAll");
}

u32 GameDataConst::getIncludedGrandGalaxyId(const char* pGalaxyName) {
    JMapInfo info;
    info.attach(&cGalaxyInfoTable);

    JMapInfoIter element = info.findElement< const char* >("name", pGalaxyName, 0);

    u32 grandGalaxyNo = 0;
    element.getValue< u32 >("GrandGalaxyNo", &grandGalaxyNo);

    return grandGalaxyNo;
}

bool GameDataConst::isPowerStarSpecial(const char* pGalaxyName, s32 starId, const char* pSpecialStarName) {
    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor accessor(iter.getFlag());

        if (!accessor.isTypeSpecialStar()) {
            continue;
        }

        if (accessor.getStarId() != starId) {
            continue;
        }

        if (!MR::isEqualString(accessor.getGalaxyName(), pGalaxyName)) {
            continue;
        }

        if (strstr(accessor.getName(), pSpecialStarName) == nullptr) {
            continue;
        }

        return true;
    }

    return false;
}
