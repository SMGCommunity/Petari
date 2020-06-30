#pragma once

#include "Actor/Light/LightZoneDataHolder.h"

class LiveActor;

class ActorLightCtrl
{
public:
    ActorLightCtrl(const LiveActor *);

    void init(s32, bool);
    void update(bool);
    void loadLight() const;
    void reset();
    void copy(const ActorLightCtrl *);

    void initActorLightInfo();

    const LiveActor* mActor; // _0
    s32 _4;
    u32* _8; // DrawBuffer*
    u32 _C;
    u32* _10; // AreaLightInfo*
    ZoneLightID mZoneLightID; // _14
    u32 _1C;
    u8 _20[0x58-0x20];
};