#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaPlanetShadow : public LiveActor {
public:
    KoopaPlanetShadow(const Koopa*);

    virtual ~KoopaPlanetShadow();
    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();

    const Koopa* mKoopa;  // 0x8C
    MtxPtr mHipMtx;       // 0x90
};
