#include "Game/AreaObj/LightArea.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include <cstdio>
#include <cstring>

void LightFunction::initLightData() {
    reinterpret_cast<LightDirector*>(MR::getSceneObjHolder()->getObj(SceneObj_LightDirector))->initData();
}

ResourceHolder* LightFunction::loadLightArchive() {
    char buf[0x100];
    snprintf(buf, 0x100, "LightData.arc");
    return MR::createAndAddResourceHolder(buf);
}

// LightFunction::createLightDataParser
// LightFunction::createZoneDataParser

void LightFunction::getAreaLightLightData(JMapInfo *pInfo, int idx, AreaLightInfo *pLightInfo) {
    MR::getCsvDataStr(&pLightInfo->mAreaLightName, pInfo, "AreaLightName", idx);
    MR::getCsvDataS32(&pLightInfo->mInterpolate, pInfo, "Interpolate", idx);
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mPlayerLight, "Player");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mStrongLight, "Strong");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mWeakLight, "Weak");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mPlanetLight, "Planet");
    MR::getCsvDataBool(&pLightInfo->mFix, pInfo, "Fix", idx);
}
