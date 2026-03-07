#pragma once

#include "Game/MapObj/MapObjActor.hpp"

struct HaloParam {
    const char* haloName;  // 0x0
    f32 _4;
    f32 _8;
    f32 clippingRadius;  // 0xC
};

class Halo : public MapObjActor {
public:
    Halo(const char*);

    virtual ~Halo();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void connectToScene(const MapObjActorInitInfo&);
    virtual f32 getDistance() const;

    bool isDistanceAppear() const;
    bool isDistanceDisappear() const;
    void exeAppear();
    void exeDisappear();

    f32 mDistance;  // 0xC4
};

class PowerStarHalo : public Halo {
public:
    PowerStarHalo(const char*);

    virtual ~PowerStarHalo();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual f32 getDistance() const;

    void exeWaitScenarioOpeningEnd();

    s32 _C8;
};
