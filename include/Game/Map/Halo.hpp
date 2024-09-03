#pragma once

#include "Game/MapObj/MapObjActor.hpp"

struct HaloParam {
    const char* haloName;   // 0x0
    f32 _4;
    f32 _8;
    f32 clippingRadius;     // 0xC
};

namespace {
    HaloParam sParams[2] = {
        { "ZoneHalo", 70.0f, 20.0f, 4.0f },
        { "PowerStarHalo", 80.0f, 20.0f, 30.0f }
    };
};

class Halo : public MapObjActor {
public:
    Halo(const char *);

    virtual ~Halo();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void connectToScene(const MapObjActorInitInfo &);
    virtual f32 getDistance() const;

    bool isDistanceAppear() const;
    bool isDistanceDisappear() const;
    void exeDisappear();

    inline HaloParam* getParam() const {
        const char* objName = mObjectName;
        for (u32 i = 0; i < 2; i++) {
            HaloParam* curParam = &sParams[i];
            if (MR::isEqualString(objName, curParam->haloName)) {
                return curParam;
            }
        }

        return NULL;
    }
    
    f32 mDistance;  // 0xC4
};

class PowerStarHalo : public Halo {
public:
    PowerStarHalo(const char *);

    virtual ~PowerStarHalo();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual f32 getDistance() const;

    void exeWaitScenarioOpeningEnd();

    s32 _C8;
};

namespace NrvHalo {
    NERVE(HostTypeAppear);
    NERVE(HostTypeDisappear);
    INIT_NERVE(HostTypeAppear);
    INIT_NERVE(HostTypeDisappear);
};

namespace NrvPowerStarHalo {
    NERVE(HostTypeWaitScenarioOpeningEnd);
    INIT_NERVE(HostTypeWaitScenarioOpeningEnd);
};
