#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoPowerUp : public ActorStateBase< Koopa > {
public:
    KoopaDemoPowerUp(Koopa* pKoopa);

    /* 0x08 */ virtual ~KoopaDemoPowerUp();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x14 */ virtual void kill();

    void exeWaitDemo();
    void exeDemo();
};
