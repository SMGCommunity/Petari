#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaStateDamageEscape;
class KoopaStateGuard;

class KoopaBattleBase : public ActorStateBase< Koopa > {
public:
    KoopaBattleBase(const char*, Koopa*);

    virtual ~KoopaBattleBase();
    virtual void init();
    virtual void kill();

    void updateChasePlayer(const MR::ActorMoveParam&);
    bool updateWander(const MR::ActorMoveParam&);
    bool updateSearch();
    bool updateFind();
    bool updateRecover(const Nerve*);

    /* 0x10 */ KoopaStateDamageEscape* mStateDamageEscape;
    /* 0x14 */ KoopaStateGuard* mStateGuard;
    /* 0x18 */ u32 _18;
};
