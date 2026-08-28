#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaPlanetShadow : public LiveActor {
public:
    KoopaPlanetShadow(const Koopa* pKoopa);

    /* 0x08 */ virtual ~KoopaPlanetShadow();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    /* 0x8C */ const Koopa* mKoopa;
    /* 0x90 */ MtxPtr mHipMtx;
};
