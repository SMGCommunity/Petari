#pragma once

#include "Game/Boss/Koopa.hpp"

class KoopaDemoPowerUp : public NerveExecutor {
public:
    KoopaDemoPowerUp(Koopa*);

    virtual void init();
    virtual void appear();
    virtual void kill();

    void exeWaitDemo();
    void exeDemo();

    /* 0x8 */ bool _8;
    /* 0C */ Koopa* mParent;
};
