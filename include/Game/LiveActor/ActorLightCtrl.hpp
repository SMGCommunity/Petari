#pragma once

#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightZoneDataHolder.hpp"

class LiveActor;
class DrawBuffer;

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
    void initActorLightInfo() NO_INLINE;
    void tryFindNewAreaLight(bool);
    void updateLightBlend();
    void resetCurrentLightInfo();
    const ActorLightInfo* getTargetActorLight(const AreaLightInfo*) const;

    /* 0x00 */ const LiveActor* mActor;
    /* 0x04 */ s32 _4;
    /* 0x08 */ DrawBuffer* _8;
    /* 0x0C */ u8 _C;
    /* 0x10 */ AreaLightInfo* mAreaLightInf;
    /* 0x14 */ ZoneLightID mLightID;
    /* 0x1C */ const ActorLightInfo* _1C;
    /* 0x20 */ ActorLightInfo mLightInfo;
    /* 0x50 */ s32 mInterpolate;
    /* 0x54 */ s32 _54;
};
