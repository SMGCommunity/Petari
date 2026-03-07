#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LavaStrangeRock : public LiveActor {
public:
    LavaStrangeRock(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void initModelAndClipping(const JMapInfoIter&);
    void initEffect();
    void exeWait();

    /* 0x8C */ s32 mRockType;
    /* 0x90 */ s32 _90;
    /* 0x94 */ LodCtrl* mLodCtrlPlanet;
};
