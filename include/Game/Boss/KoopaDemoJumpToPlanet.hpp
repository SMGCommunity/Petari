#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoJumpToPlanet : public ActorStateBase< Koopa > {
public:
    KoopaDemoJumpToPlanet(Koopa* pKoopa);

    /* 0x08 */ virtual ~KoopaDemoJumpToPlanet();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void kill();

    void startReady();

    void exeStart();
    void exeFall();
    void exeLand();
    void exeWaitPlayer();
};
