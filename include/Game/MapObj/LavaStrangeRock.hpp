#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/Util/JMapInfo.hpp"
class LavaStrangeRock : public LiveActor {
    public:
    LavaStrangeRock(const char*);

    s32 rockType;  //0x8C
    s32 _90;  //0x90
    LodCtrl* mLodCtrlPlanet;  //0x94

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void initModelAndClipping(const JMapInfoIter&);
    void initEffect();
    void exeWait();
};
