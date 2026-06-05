#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

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
