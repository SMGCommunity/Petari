#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaPlanetShadow : public LiveActor {
public:
    KoopaPlanetShadow(const Koopa* pKoopa);

    virtual ~KoopaPlanetShadow();
    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAndSetBaseMtx();

    /* 0x8C */ const Koopa* mKoopa;
    /* 0x90 */ MtxPtr mHipMtx;
};
