#include "Game/System/GameDataConst.hpp"
#include "Game/System/GameEventFlag.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/Util.hpp"
#include <cstdio>

s32 GameDataConst::getPowerStarNumToOpenGalaxy(const char* pGalaxy) {
    JMapInfo info;
    info.attach(&GalaxyIDBCSV);
    JMapInfoIter element;
    element = info.findElement< const char* >("name", pGalaxy, 0);
    u32 powerStarNum = 0;
    element.getValue< u32 >("PowerStarNum", &powerStarNum);
    return powerStarNum;
}

bool GameDataConst::isPowerStarGreen(const char* pGalaxy, s32 starId) {
    return isPowerStarSpecial(pGalaxy, starId, "SpecialStarGreen");
}

bool GameDataConst::isPowerStarRed(const char* pGalaxy, s32 starId) {
    return isPowerStarSpecial(pGalaxy, starId, "SpecialStarRed");
}

bool GameDataConst::isPowerStarLuigiHas(const char* pGalaxy, s32 starId) {
    return isPowerStarSpecial(pGalaxy, starId, "SpecialStarFindingLuigi");
}

bool GameDataConst::isGrandStar(const char* pGalaxy, s32 starId) {
    return isPowerStarSpecial(pGalaxy, starId, "SpecialStarGrand");
}

bool GameDataConst::isGalaxyLuigiArrested(const char* pGalaxy, s32 starId) {
    GameEventFlagAccessor accessor = GameEventFlagTable::makeAccessor("SpecialStarLuigiRescued");
    bool ret = false;
    const char* galaxyName = accessor.getGalaxyName();

    if (MR::isEqualString(galaxyName, pGalaxy) && ((starId == -1) || starId == accessor.getStarId())) {
        ret = true;
    }

    return ret;
}

bool GameDataConst::isQuestionGalaxy(const char* pGalaxy) {
    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor accessor(iter.getFlag());

        if (accessor.isTypeGalaxyOpenStar()) {
            char dest[0x40];
            snprintf(dest, sizeof(dest), "Appear%s", pGalaxy);

            if (MR::isEqualString(accessor.getName(), dest)) {
                return true;
            }
        }
    }
    return false;
}

bool GameDataConst::isGalaxyAppearGreenDriver(const char* pGalaxyName) {
    char buf[0x30];
    snprintf(buf, sizeof(buf), "Appear%s", pGalaxyName);
    return !GameEventFlagTable::isExist(buf) ? false : GameEventFlagTable::isDependedAnother(buf, "SpecialStarGreenAll");
}

u32 GameDataConst::getIncludedGrandGalaxyId(const char* pGalaxy) {
    JMapInfo info;
    info.attach(&GalaxyIDBCSV);
    JMapInfoIter element;
    element = info.findElement< const char* >("name", pGalaxy, 0);
    u32 grandGalaxyNo = 0;
    element.getValue< u32 >("GrandGalaxyNo", &grandGalaxyNo);
    return grandGalaxyNo;
}

bool GameDataConst::isPowerStarSpecial(const char* pGalaxy, s32 starId, const char* pSpecial) {
    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor accessor(iter.getFlag());
        
        if (accessor.isTypeSpecialStar() && accessor.getStarId() == starId) {
            if (MR::isEqualString(accessor.getGalaxyName(), pGalaxy) && strstr(accessor.getName(), pSpecial) != nullptr) {
                return true;
            }
        }
    }
    return false;
}
