#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoFallToPlanetVs3Lv3 : public ActorStateBase< Koopa > {
public:
    KoopaDemoFallToPlanetVs3Lv3(Koopa* pKoopa);

    virtual ~KoopaDemoFallToPlanetVs3Lv3();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x14 */ virtual void kill();

    void exeWaitFallDemo();
    void exeFallOutside();
    void exeFallInside();
    void exeLand();
    void exeWaitPlayer();
};
