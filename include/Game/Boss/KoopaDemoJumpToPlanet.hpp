#pragma once

#include "Game/Boss/Koopa.hpp"

class KoopaDemoJumpToPlanet : public NerveExecutor {
public:
    KoopaDemoJumpToPlanet(Koopa*);

    virtual void init();
    virtual void kill();

    void startReady();
    void exeStart();
    void exeFall();

    /* 0x8 */ bool _8;
    /* 0C */ Koopa* mParent;
};
