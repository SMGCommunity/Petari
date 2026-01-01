#include "Game/Map/LightFunction.hpp"
#include "Game/AreaObj/LightArea.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include <cstdio>
#include <cstring>

namespace {
    void loadLightDiffuse(_GXColor color, const TVec3f& rPos, _GXLightID lightID) {
        GXLightObj lightObj;

        GXInitLightPos(&lightObj, rPos.x, rPos.y, rPos.z);
        GXInitLightAttn(&lightObj, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

        _GXColor new_color(color);
        GXInitLightColor(&lightObj, new_color);
        GXLoadLightObjImm(&lightObj, lightID);
    }

    void getDataActorLightInfo(JMapInfo*, int, ActorLightInfo*, const char*);
};  // namespace

void LightFunction::initLightData() {
    MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->initData();
}

ResourceHolder* LightFunction::loadLightArchive() {
    char buf[0x100];
    snprintf(buf, sizeof(buf), "LightData.arc");
    return MR::createAndAddResourceHolder(buf);
}

s32 LightFunction::createLightDataParser(JMapInfo** pOut) {
    ResourceHolder* holder = MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mResourceHolder;
    JMapInfo* info = MR::tryCreateCsvParser(holder, "LightData.bcsv");
    *pOut = info;
    if (info->mData != nullptr) {
        return info->mData->mNumEntries;
    }

    return 0;
}

s32 LightFunction::createZoneDataParser(const char* pZone, JMapInfo** pOut) {
    ResourceHolder* holder = MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mResourceHolder;
    JMapInfo* info = MR::tryCreateCsvParser(holder, "Light%s.bcsv", pZone);
    *pOut = info;

    if (info != nullptr) {
        if (info->mData != nullptr) {
            return info->mData->mNumEntries;
        }

        return 0;
    }

    return 0;
}

void LightFunction::getAreaLightLightData(JMapInfo* pInfo, int idx, AreaLightInfo* pLightInfo) {
    MR::getCsvDataStr(&pLightInfo->mAreaLightName, pInfo, "AreaLightName", idx);
    MR::getCsvDataS32(&pLightInfo->mInterpolate, pInfo, "Interpolate", idx);
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mPlayerLight, "Player");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mStrongLight, "Strong");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mWeakLight, "Weak");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mPlanetLight, "Planet");
    MR::getCsvDataBool(&pLightInfo->mFix, pInfo, "Fix", idx);
}
