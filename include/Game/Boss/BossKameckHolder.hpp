#pragma once

#include "Game/Boss/BossKameck.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class BossKameck;

class BossKameckStateHolder : public ActorStateBase<BossKameck> {
public:
    inline BossKameckStateHolder(BossKameck *pBoss, const char *pName) : ActorStateBase<BossKameck>(pName) {
        mBossKameck = pBoss;
    }

    BossKameck* mBossKameck;            // _0
};