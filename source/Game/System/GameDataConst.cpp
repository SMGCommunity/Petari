#include "Game/System/GameDataConst.h"
#include "Game/System/GameEventFlag.h"
#include "Game/System/GameEventFlagTable.h"
#include "Game/Util.h"
#include <cstdio>

u32 GameDataConst::getPowerStarNumToOpenGalaxy(const char *pGalaxy) {
    JMapInfo info;
    info.attach(&GalaxyIDBCSV);
    JMapInfoIter element;
    element = info.findElement<const char *>("name", pGalaxy, 0);
    u32 powerStarNum = 0;
    element.getValue<u32>("PowerStarNum", &powerStarNum);
    return powerStarNum;
}

bool GameDataConst::isPowerStarGreen(const char *pName, s32 a2) {
    return isPowerStarSpecial(pName, a2, "SpecialStarGreen");
}

bool GameDataConst::isPowerStarRed(const char *pName, s32 a2) {
    return isPowerStarSpecial(pName, a2, "SpecialStarRed");
}

bool GameDataConst::isPowerStarLuigiHas(const char *pName, s32 a2) {
    return isPowerStarSpecial(pName, a2, "SpecialStarFindingLuigi");
}

bool GameDataConst::isGrandStar(const char *pName, s32 a2) {
    return isPowerStarSpecial(pName, a2, "SpecialStarGrand");
}

bool GameDataConst::isGalaxyLuigiArrested(const char *pGalaxy, s32 starId) {
    GameEventFlagAccessor accessor = GameEventFlagTable::makeAccessor("SpecialStarLuigiRescued");
    bool ret = false;
    const char* galaxyName = accessor.getGalaxyName();

    if (MR::isEqualString(galaxyName, pGalaxy) && ((starId == -1) || starId == accessor.getStarId())) {
        ret = true;
    } 

    return ret;
}
 
bool GameDataConst::isGalaxyAppearGreenDriver(const char *pGalaxyName) {
    char buf[0x30];
    snprintf(buf, 0x30, "Appear%s", pGalaxyName);
    return !GameEventFlagTable::isExist(buf) ? false : GameEventFlagTable::isDependedAnother(buf, "SpecialStarGreenAll");
}

u32 GameDataConst::getIncludedGrandGalaxyId(const char *pGalaxy) {
    JMapInfo info;
    info.attach(&GalaxyIDBCSV);
    JMapInfoIter element;
    element = info.findElement<const char *>("name", pGalaxy, 0);
    u32 grandGalaxyNo = 0;
    element.getValue<u32>("GrandGalaxyNo", &grandGalaxyNo);
    return grandGalaxyNo;
}