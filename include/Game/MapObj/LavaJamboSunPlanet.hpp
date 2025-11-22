#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class PartsModel;

class LavaJamboSunPlanet : public LiveActor {
public:
    LavaJamboSunPlanet(const char*);

    virtual ~LavaJamboSunPlanet();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();

    void exeWait();
    void exeMove();

    PartsModel* mBloomModel;
    LodCtrl* mLodCtrl;
    f32 _94;
    TVec3f _98;
};
