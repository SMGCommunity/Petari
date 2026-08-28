#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class HitSensor;
class Koopa;

class KoopaStateGuard : public ActorStateBase< Koopa > {
public:
    KoopaStateGuard(Koopa* pKoopa);

    /* 0x08 */ virtual ~KoopaStateGuard();
    /* 0x0C */ virtual void init();

    bool tryStart(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeGuardFace();
    void exeGuardBody();
    void exeGuardTail();
};
