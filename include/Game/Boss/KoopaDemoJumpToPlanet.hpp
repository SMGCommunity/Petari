#pragma once

#include "Game/System/NerveExecutor.hpp"

class Koopa;

class KoopaDemoJumpToPlanet : public NerveExecutor {
public:
    KoopaDemoJumpToPlanet(Koopa*);

    virtual ~KoopaDemoJumpToPlanet();
    virtual void init();

    private:
    /* 0x8 */ u32 _8;
    /* 0xC */ Koopa* mKoopa;
};
