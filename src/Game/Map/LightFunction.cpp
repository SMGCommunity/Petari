#include "Game/Map/LightFunction.hpp"
#include "Game/AreaObj/LightArea.hpp"
#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include "revolution/gx/GXEnum.h"
#include <cstdio>
#include <cstring>

const GXLightID cLightDataIDs[8] = {GX_LIGHT0, GX_LIGHT1, GX_LIGHT2, GX_LIGHT3, GX_LIGHT4, GX_LIGHT5, GX_LIGHT6, GX_LIGHT7};

namespace {
    void loadLightDiffuse(_GXColor color, const TVec3f& rPos, _GXLightID lightID) {
        GXLightObj lightObj;

        GXInitLightPos(&lightObj, rPos.x, rPos.y, rPos.z);
        GXInitLightAttn(&lightObj, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

        _GXColor new_color(color);
        GXInitLightColor(&lightObj, new_color);
        GXLoadLightObjImm(&lightObj, lightID);
    }
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

namespace {
    void getDataLightInfo(JMapInfo* pInfo, int id, LightInfo* pLightInfo, const char* pName) {
        char colorBuf[256];
        snprintf(colorBuf, sizeof(colorBuf), "%sColor", pName);
        MR::getCsvDataColor(&pLightInfo->mColor, pInfo, colorBuf, id);
        char posBuf[256];
        snprintf(posBuf, sizeof(posBuf), "%sPos", pName);
        MR::getCsvDataVec(&pLightInfo->mPos, pInfo, posBuf, id);
        char camBuf[128];
        snprintf(camBuf, sizeof(camBuf), "%sFollowCamra", pName);
        MR::getCsvDataBool(&pLightInfo->mIsFollowCamera, pInfo, camBuf, id);
    }

    void getDataActorLightInfo(JMapInfo* pInfo, int lightID, ActorLightInfo* pActorInfo, const char* pName) {
        char lightBuf[256];
        snprintf(lightBuf, sizeof(lightBuf), "%sLight%d", pName, 0);
        getDataLightInfo(pInfo, lightID, &pActorInfo->mInfo0, lightBuf);
        snprintf(lightBuf, sizeof(lightBuf), "%sLight%d", pName, 1);
        getDataLightInfo(pInfo, lightID, &pActorInfo->mInfo1, lightBuf);
        char alphaBuf[256];
        snprintf(alphaBuf, sizeof(alphaBuf), "%sAlpha2", pName);
        MR::getCsvDataU8(&pActorInfo->mAlpha2, pInfo, alphaBuf, lightID);
        char ambBuf[256];
        snprintf(ambBuf, sizeof(ambBuf), "%sAmbient", pName);
        MR::getCsvDataColor(&pActorInfo->mColor, pInfo, ambBuf, lightID);
    }
};  // namespace

void LightFunction::getAreaLightLightData(JMapInfo* pInfo, int idx, AreaLightInfo* pLightInfo) {
    MR::getCsvDataStr(&pLightInfo->mAreaLightName, pInfo, "AreaLightName", idx);
    MR::getCsvDataS32(&pLightInfo->mInterpolate, pInfo, "Interpolate", idx);
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mPlayerLight, "Player");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mStrongLight, "Strong");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mWeakLight, "Weak");
    getDataActorLightInfo(pInfo, idx, &pLightInfo->mPlanetLight, "Planet");
    MR::getCsvDataBool(&pLightInfo->mFix, pInfo, "Fix", idx);
}
