#pragma once

#include "Game/MapObj/MapObjActor.hpp"

struct HaloParam {
    /* 0x0 */ const char* mName;
    /* 0x4 */ f32 _4;
    /* 0x8 */ f32 _8;
    /* 0xC */ f32 mClippingRadius;
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

    /* 0xC4 */ f32 mDistance;
};

class PowerStarHalo : public Halo {
public:
    PowerStarHalo(const char*);

    virtual ~PowerStarHalo();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual f32 getDistance() const;

    void exeWaitScenarioOpeningEnd();

    /* 0xC8 */ s32 mAppearDuringOpeningCam;
};
