#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoFallToPlanetVs3Lv3 : public ActorStateBase< Koopa > {
public:
    KoopaDemoFallToPlanetVs3Lv3(Koopa* pKoopa);

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
