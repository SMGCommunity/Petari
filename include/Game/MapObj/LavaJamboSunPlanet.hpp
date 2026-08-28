#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;
class PartsModel;

class LavaJamboSunPlanet : public LiveActor {
public:
    LavaJamboSunPlanet(const char*);

    /* 0x08 */ virtual ~LavaJamboSunPlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();

    void exeWait();
    void exeMove();

    PartsModel* mBloomModel;
    LodCtrl* mLodCtrl;
    f32 _94;
    TVec3f _98;
};
