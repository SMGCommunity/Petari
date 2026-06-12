#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaStateAttackFireLong : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackFireLong(Koopa* pKoopa);

    virtual ~KoopaStateAttackFireLong();
    virtual void init();
    virtual void appear();

    void exeStart();
    void exeEmit();

    /* 0x10 */ s32 mMaxFire;
    /* 0x14 */ s32 mFireEmitted;
};
