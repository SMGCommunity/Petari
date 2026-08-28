#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"

class LavaGalaxyParts : public LiveActor {
public:
    LavaGalaxyParts(const char*);

    /* 0x08 */ virtual ~LavaGalaxyParts();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x48 */ virtual void control();

    LodCtrl* mLodCtrl;                   // 0x8C
    ProjmapEffectMtxSetter* mMtxSetter;  // 0x90
};