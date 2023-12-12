#pragma once

#include <revolution.h>
#include "Game/Map/LightDataHolder.h"
#include "Game/Map/LightZoneDataHolder.h"
#include "JSystem/JGeometry/TVec.h"

class JMapInfo;
class LightAreaHolder;
struct PointLightInfo;
class ResourceHolder;
class ActorLightCtrl;

namespace {
    void loadLightDiffuse(_GXColor color, const TVec3f &rPos, _GXLightID lightID) {
        GXLightObj lightObj;

        GXInitLightPos(&lightObj, rPos.x, rPos.y, rPos.z);
        GXInitLightAttn(&lightObj, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

        _GXColor new_color(color);
        GXInitLightColor(&lightObj, new_color);
        GXLoadLightObjImm(&lightObj, lightID);
    }

    void getDataActorLightInfo(JMapInfo *, int, ActorLightInfo *, const char *);
};

class LightFunction {
public:
    static void initLightRegisterAll();
    static void initLightData();
    static ResourceHolder* loadLightArchive();
    static s32 createLightDataParser(JMapInfo **);

    static void loadAllLightWhite();

    static AreaLightInfo* getAreaLightInfo(const ZoneLightID &);

    static void loadActorLightInfo(const ActorLightInfo *);

    static void getAreaLightLightData(JMapInfo *, int, AreaLightInfo *);
    static const char* getDefaultAreaLightName();

    static s32 createZoneDataParser(const char *, JMapInfo **);

    static void loadPointLightInfo(const PointLightInfo *);

    static void loadLightInfoCoin(const LightInfoCoin *);

    static void registerLightAreaHolder(LightAreaHolder *);

    static void calcLightWorldPos(TVec3f *, const LightInfo &);

    static void registerPlayerLightCtrl(const ActorLightCtrl *);
};
