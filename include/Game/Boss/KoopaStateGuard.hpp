#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaStateGuard : public ActorStateBase< Koopa > {
public:
    KoopaStateGuard(Koopa* pKoopa);

    virtual ~KoopaStateGuard();
    virtual void init();

    bool tryStart(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeGuardFace();
    void exeGuardBody();
    void exeGuardTail();
};
