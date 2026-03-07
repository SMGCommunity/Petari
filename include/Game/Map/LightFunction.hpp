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

    static void loadAllLightWhite();

    static AreaLightInfo* getAreaLightInfo(const ZoneLightID&);

    static void loadActorLightInfo(const ActorLightInfo*);

    static void getAreaLightLightData(JMapInfo*, int, AreaLightInfo*);
    static const char* getDefaultAreaLightName();

    static void loadPointLightInfo(const PointLightInfo*);

    static void loadLightInfoCoin(const LightInfoCoin*);

    static void registerLightAreaHolder(LightAreaHolder*);

    static void calcLightWorldPos(TVec3f*, const LightInfo&);

    static void registerPlayerLightCtrl(const ActorLightCtrl*);
};
