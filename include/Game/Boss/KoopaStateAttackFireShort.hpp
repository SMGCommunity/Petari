#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaStateAttackFireShort : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackFireShort(Koopa* pKoopa);

    virtual ~KoopaStateAttackFireShort();
    virtual void init();
    virtual void appear();

    void exeStart();
    void exeEmit();

    /* 0x10 */ s32 mFireEmitted;
    /* 0x14 */ s32 mMaxFire;
    /* 0x18 */ s32 mFireDelay;
};
