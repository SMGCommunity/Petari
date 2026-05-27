#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaStateGuard : public ActorStateBase< Koopa > {
public:
    KoopaStateGuard(Koopa*);

    virtual ~KoopaStateGuard();
    virtual void init();

    bool tryStart(u32, HitSensor*, HitSensor*);

    void exeGuardFace();
    void exeGuardBody();
    void exeGuardTail();
};
