#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaStateAttackFireShort : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackFireShort(Koopa*);

    virtual ~KoopaStateAttackFireShort();
    virtual void init();
    virtual void appear();

    void exeStart();
    void exeEmit();

    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ u32 _18;
};
