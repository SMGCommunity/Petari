#include "Game/Map/LightFunction.hpp"
#include "Game/AreaObj/LightArea.hpp"
#include "Game/AreaObj/LightAreaHolder.hpp"
#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Map/LightPointCtrl.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include "revolution/gx/GXEnum.h"
#include <cstdio>
#include <cstring>

const GXLightID cLightDataIDs[8] = {GX_LIGHT0, GX_LIGHT1, GX_LIGHT2, GX_LIGHT3, GX_LIGHT4, GX_LIGHT5, GX_LIGHT6, GX_LIGHT7};

namespace {};  // namespace

void LightFunction::initLightRegisterAll() {
    GXColor colors[4] = {{0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}};

    for (s32 i = 0; i < 8; i++) {
        GXColor c = {-1, -1, -1, -1};
        GXLightObj obj;
        GXInitLightColor(&obj, c);
        GXInitLightPos(&obj, 0.0f, 0.0f, 0.0f);
        GXInitLightDir(&obj, 0.0, -1.0, 0.0);
        GXInitLightAttn(&obj, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        GXInitSpecularDir(&obj, 0.0, -1.0, 0.0);
        GXInitSpecularDirHA(&obj, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0);
        GXInitLightAttn(&obj, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
        GXLoadLightObjImm(&obj, cLightDataIDs[i]);
    }
}

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
        snprintf(camBuf, sizeof(camBuf), "%sFollowCamera", pName);
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
    ::getDataActorLightInfo(pInfo, idx, &pLightInfo->mPlayerLight, "Player");
    ::getDataActorLightInfo(pInfo, idx, &pLightInfo->mStrongLight, "Strong");
    ::getDataActorLightInfo(pInfo, idx, &pLightInfo->mWeakLight, "Weak");
    ::getDataActorLightInfo(pInfo, idx, &pLightInfo->mPlanetLight, "Planet");
    MR::getCsvDataBool(&pLightInfo->mFix, pInfo, "Fix", idx);
}

const char* LightFunction::getDefaultAreaLightName() {
    return MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mDataHolder->getDefaultAreaLightName();
}

s32 LightFunction::getDefaultStepInterpolate() {
    return MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mDataHolder->getDefaultStepInterpolate();
}

void LightFunction::registerPlayerLightCtrl(const ActorLightCtrl* pCtrl) {
    MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->_1C = pCtrl;
}

void LightFunction::registerLightAreaHolder(LightAreaHolder* pHolder) {
    MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->_C = pHolder;
}

bool LightFunction::tryFindNewAreaLightID(const TVec3f& rPos, ZoneLightID* pId) {
    return MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->_C->tryFindLightID(rPos, pId);
}

AreaLightInfo* LightFunction::getAreaLightInfo(const ZoneLightID& rId) {
    const char* name = MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mZoneDataHolder->getAreaLightNameInZoneData(rId);
    return MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mDataHolder->findAreaLight(name);
}

namespace {
    void blendActorLightPos(const LightInfo& a1, const LightInfo& a2, LightInfo* a3, f32 a4) {
        if (!a3->mIsFollowCamera) {
            if (!a1.mIsFollowCamera) {
                TVec3f v11(a1.mPos);
                MR::getCameraViewMtx().mult(v11, v11);
                MR::blendVec(&a3->mPos, v11, a2.mPos, a4);
                return;
            }
        } else if (a1.mIsFollowCamera) {
            TVec3f v9(a1.mPos);
            MR::getCameraInvViewMtx().mult(v9, v9);
            MR::blendVec(&a3->mPos, v9, a2.mPos, a4);
            return;
        }

        MR::blendVec(&a3->mPos, a1.mPos, a2.mPos, a4);
    }
};  // namespace

void LightFunction::blendActorLightInfo(ActorLightInfo* a1, const ActorLightInfo& a2, const ActorLightInfo& a3, f32 a4) {
    MR::blendColor(&a1->mInfo0.mColor, a2.mInfo0.mColor, a3.mInfo0.mColor, a4);
    MR::blendColor(&a1->mInfo1.mColor, a2.mInfo1.mColor, a3.mInfo1.mColor, a4);
    MR::blendColor(&a1->mColor, a2.mColor, a3.mColor, a4);
    ::blendActorLightPos(a2.mInfo0, a3.mInfo0, &a1->mInfo0, a4);
    ::blendActorLightPos(a2.mInfo1, a3.mInfo1, &a1->mInfo1, a4);
    a1->mAlpha2 = MR::getInterpolateValue(a4, a2.mAlpha2, a3.mAlpha2);
}

namespace {
    void loadLightDiffuse(GXColor color, const TVec3f& rPos, GXLightID lightID) NO_INLINE {
        GXLightObj lightObj;

        GXInitLightPos(&lightObj, rPos.x, rPos.y, rPos.z);
        GXInitLightAttn(&lightObj, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

        GXColor new_color(color);
        GXInitLightColor(&lightObj, new_color);
        GXLoadLightObjImm(&lightObj, lightID);
    }

    void loadLightInfoDiffuse(const LightInfo& a1, GXLightID a2) NO_INLINE {
        if (a1.mIsFollowCamera) {
            TVec3f v13(a1.mPos);
            ::loadLightDiffuse(GXColor(a1.mColor), v13, a2);
        } else {
            TVec3f v14(a1.mPos);
            MR::getCameraViewMtx().mult(v14, v14);
            ::loadLightDiffuse(GXColor(a1.mColor), v14, a2);
        }
    }
}  // namespace

void LightFunction::loadActorLightInfo(const ActorLightInfo* pInfo) {
    ::loadLightInfoDiffuse(pInfo->mInfo0, GX_LIGHT0);
    ::loadLightInfoDiffuse(pInfo->mInfo1, GX_LIGHT1);

    GXColor c = {0, 0, 0, pInfo->mAlpha2};
    ::loadLightDiffuse(c, TVec3f(0.0f, 0.0f, 0.0f), GX_LIGHT2);
    GXSetChanAmbColor(GX_COLOR0A0, GXColor(pInfo->mColor));
}

void LightFunction::loadLightInfoCoin(const LightInfoCoin* pInfo) {
    ::loadLightInfoDiffuse(pInfo->base, GX_LIGHT0);
    TVec3f v7(0.0f, 0.0f, -1.0f);
    GXLightObj obj;
    f32 v = pInfo->_18;
    GXInitLightColor(&obj, GXColor(pInfo->_14));
    GXInitSpecularDir(&obj, v7.x, v7.y, v7.z);
    GXInitLightAttn(&obj, 0.0f, 0.0f, 1.0f, (v / 2.0f), 0.0f, (1.0f - (v / 2.0f)));
    GXLoadLightObjImm(&obj, GX_LIGHT3);
}

void LightFunction::loadPointLightInfo(const PointLightInfo* pInfo) {
    TVec3f v7(pInfo->_0);
    MR::getCameraViewMtx().mult(v7, v7);
    GXLightObj obj;
    GXInitLightPos(&obj, v7.x, v7.y, v7.z);
    GXInitLightDistAttn(&obj, pInfo->_10, pInfo->_14, pInfo->_18);
    GXInitLightSpot(&obj, 0.0, GX_SP_OFF);
    GXInitLightColor(&obj, GXColor(pInfo->_C));
    GXLoadLightObjImm(&obj, GX_LIGHT4);
}

// LightFunction::loadAllLightWhite

void LightFunction::calcLightWorldPos(TVec3f* pPos, const LightInfo& rInfo) {
    pPos->x = rInfo.mPos.x;
    pPos->y = rInfo.mPos.y;
    pPos->z = rInfo.mPos.z;

    if (rInfo.mIsFollowCamera) {
        TPos3f mtx;
        mtx.setInline(MR::getCameraViewMtx());
        mtx.invert(mtx);
        mtx.mult(*pPos, *pPos);
    }
}
