#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoPowerUp : public ActorStateBase< Koopa > {
public:
    KoopaDemoPowerUp(Koopa* pKoopa);

    virtual ~KoopaDemoPowerUp();
    virtual void init();
    virtual void appear();
    virtual void kill();

    void exeWaitDemo();
    void exeDemo();
};
