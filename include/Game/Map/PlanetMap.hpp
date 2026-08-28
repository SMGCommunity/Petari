#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class LodCtrl;
class ModelObj;
class PartsModel;

class PlanetMap : public LiveActor {
public:
    PlanetMap(const char*, const char*);

    /* 0x08 */ virtual ~PlanetMap();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();
    virtual f32 getFarClipDistance() const;
    virtual s32 getLowMovementType() const;

    void initClipping(const JMapInfoIter&);
    void initModel(const char*, const JMapInfoIter&);
    void initBloomModel(const char*);
    bool tryEmitMyEffect();
    bool tryDeleteMyEffect();

    /* 0x8C */ const char* mModelName;
    TVec3f _90;
    /* 0x9C */ LodCtrl* mLODCtrl;
    /* 0xA0 */ ModelObj* mBloomModel;
    /* 0xA4 */ PartsModel* mWaterModel;
    /* 0xA8 */ PartsModel* mIndirectModel;
};

class FurPlanetMap : public PlanetMap {
public:
    FurPlanetMap(const char*);

    /* 0x08 */ virtual ~FurPlanetMap();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};

class RailPlanetMap : public PlanetMap {
public:
    RailPlanetMap(const char*);

    /* 0x08 */ virtual ~RailPlanetMap();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};

class PlanetMapAnimLow : public PlanetMap {
public:
    PlanetMapAnimLow();

    /* 0x08 */ virtual ~PlanetMapAnimLow();
    virtual s32 getLowMovementType() const;
};

struct PlanetMapClippingInfo {
    const char* mName;
    f32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
    s32 _14;
};
