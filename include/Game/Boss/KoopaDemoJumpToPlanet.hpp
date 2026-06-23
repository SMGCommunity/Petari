#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoJumpToPlanet : public ActorStateBase< Koopa > {
public:
    KoopaDemoJumpToPlanet(Koopa* pKoopa);

    virtual ~KoopaDemoJumpToPlanet();
    virtual void init();
    virtual void kill();

    void startReady();

    void exeStart();
    void exeFall();
    void exeLand();
    void exeWaitPlayer();
};
