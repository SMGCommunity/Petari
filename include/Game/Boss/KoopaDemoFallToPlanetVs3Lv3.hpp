#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaDemoFallToPlanetVs3Lv3 : public ActorStateBase< Koopa > {
public:
    KoopaDemoFallToPlanetVs3Lv3(Koopa*);

    virtual ~KoopaDemoFallToPlanetVs3Lv3();
    virtual void init();
    virtual void appear();
    virtual void kill();

    void exeWaitFallDemo();
    void exeFallOutside();
    void exeFallInside();
    void exeLand();
    void exeWaitPlayer();
};
