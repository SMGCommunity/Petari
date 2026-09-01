#pragma once

#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightZoneDataHolder.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class JMapInfo;
class LightAreaHolder;
struct PointLightInfo;
class ResourceHolder;
class ActorLightCtrl;

class LightFunction {
public:
    static void initLightRegisterAll();
    static void initLightData();
    static ResourceHolder* loadLightArchive();
    static s32 createLightDataParser(JMapInfo**);
    static s32 createZoneDataParser(const char*, JMapInfo**);
    static void getAreaLightLightData(JMapInfo*, int, AreaLightInfo*);
    static const char* getDefaultAreaLightName();
    static s32 getDefaultStepInterpolate();
    static void registerPlayerLightCtrl(const ActorLightCtrl*);
    static void registerLightAreaHolder(LightAreaHolder*);
    static bool tryFindNewAreaLightID(const TVec3f&, ZoneLightID*);
    static AreaLightInfo* getAreaLightInfo(const ZoneLightID&);
    static void blendActorLightInfo(ActorLightInfo*, const ActorLightInfo&, const ActorLightInfo&, f32);
    static void loadActorLightInfo(const ActorLightInfo*);
    static void loadLightInfoCoin(const LightInfoCoin*);
    static void loadPointLightInfo(const PointLightInfo*);
    static void loadAllLightWhite();
    static void calcLightWorldPos(TVec3f*, const LightInfo&);
};
