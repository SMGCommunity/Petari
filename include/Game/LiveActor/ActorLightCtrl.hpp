#pragma once

#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightZoneDataHolder.hpp"

class LiveActor;

class ActorLightCtrl {
public:
    ActorLightCtrl(const LiveActor*);

    void init(int, bool);
    void update(bool);
    void loadLight() const;
    void reset();
    void copy(const ActorLightCtrl*);
    bool isSameLight(const ActorLightCtrl*) const;
    const ActorLightInfo* getActorLight() const;
    void initActorLightInfo();
    void tryFindNewAreaLight(bool);
    void updateLightBlend();
    void resetCurrentLightInfo();
    const ActorLightInfo* getTargetActorLight(const AreaLightInfo*) const;

    const LiveActor* mActor;  // 0x0
    s32 _4;
    u32 _8;
    u8 _C;
    AreaLightInfo* mAreaLightInf;  // 0x10
    ZoneLightID mLightID;          // 0x14
    u32 _1C;
    ActorLightInfo mLightInfo;  // 0x20
    s32 mInterpolate;           // 0x50
    s32 _54;
};