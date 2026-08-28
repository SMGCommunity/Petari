#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoFallToPlanetVs3Lv2 : public ActorStateBase< Koopa > {
public:
    KoopaDemoFallToPlanetVs3Lv2(Koopa* pKoopa);

    virtual ~KoopaDemoFallToPlanetVs3Lv2();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x14 */ virtual void kill();

    void exeWaitFall();
    void exeDemoFall();
    void exeFallToLand();
    void exeLand();
    void exeWaitPlayer();
};
