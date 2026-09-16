#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;
class ProjmapEffectMtxSetter;

class LavaGalaxyParts : public LiveActor {
public:
    LavaGalaxyParts(const char*);

    virtual ~LavaGalaxyParts();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();

    /* 0x8C */ LodCtrl* mLodCtrl;
    /* 0x90 */ ProjmapEffectMtxSetter* mMtxSetter;
};
