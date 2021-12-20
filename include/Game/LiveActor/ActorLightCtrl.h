#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Map/LightZoneDataHolder.h"
#include "Game/Map/LightDataHolder.h"

class ActorLightCtrl {
public:
    ActorLightCtrl(const LiveActor *);

    void init(int, bool);
    void update(bool);
    void loadLight() const;
    void reset();
    void copy(const ActorLightCtrl *);
    bool isSameLight(const ActorLightCtrl *) const;
    const ActorLightInfo* getActorLight() const;
    void initActorLightInfo();
    void tryFindNewAreaLight(bool);
    void updateLightBlend();
    void resetCurrentLightInfo();
    const ActorLightInfo* getTargetActorLight(const AreaLightInfo *) const;

    const LiveActor* mActor;        // _0
    s32 _4;
    u32 _8;
    u8 _C;
    AreaLightInfo* mAreaLightInf;    // _10
    ZoneLightID mLightID;           // _14
    u32 _1C;
    ActorLightInfo mLightInfo;      // _20
    s32 mInterpolate;               // _50
    s32 _54;
};