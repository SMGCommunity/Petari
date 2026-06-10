#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoFallToPlanetVs3Lv2 : public ActorStateBase< Koopa > {
public:
    KoopaDemoFallToPlanetVs3Lv2(Koopa* pKoopa);

    virtual ~KoopaDemoFallToPlanetVs3Lv2();
    virtual void init();
    virtual void appear();
    virtual void kill();

    void exeWaitFall();
    void exeDemoFall();
    void exeFallToLand();
    void exeLand();
    void exeWaitPlayer();
};
