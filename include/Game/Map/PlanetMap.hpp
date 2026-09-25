#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class LodCtrl;
class ModelObj;
class PartsModel;

class PlanetMap : public LiveActor {
public:
    PlanetMap(const char*, const char* = nullptr);

    virtual ~PlanetMap();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual f32 getFarClipDistance() const;
    virtual s32 getLowMovementType() const;

    void initClipping(const JMapInfoIter&);
    void initModel(const char*, const JMapInfoIter&);
    void initBloomModel(const char*);
    bool tryEmitMyEffect();
    bool tryDeleteMyEffect();

    /* 0x8C */ const char* mModelName;
    /* 0x90 */ TVec3f _90;
    /* 0x9C */ LodCtrl* mLODCtrl;
    /* 0xA0 */ ModelObj* mBloomModel;
    /* 0xA4 */ PartsModel* mWaterModel;
    /* 0xA8 */ PartsModel* mIndirectModel;
};

class FurPlanetMap : public PlanetMap {
public:
    FurPlanetMap(const char*);

    virtual ~FurPlanetMap();
    virtual void init(const JMapInfoIter&);
};

class RailPlanetMap : public PlanetMap {
public:
    RailPlanetMap(const char*);

    virtual ~RailPlanetMap();
    virtual void init(const JMapInfoIter&);
};

class PlanetMapAnimLow : public PlanetMap {
public:
    PlanetMapAnimLow(const char* p1) : PlanetMap(p1) {
    }

    virtual ~PlanetMapAnimLow();
    virtual s32 getLowMovementType() const;
};

struct PlanetMapClippingInfo {
    /* 0x00 */ const char* mName;
    /* 0x04 */ f32 mRadius;
    /* 0x08 */ Vec mOffset;
    /* 0x14 */ s32 _14;
};
