#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class HitSensor;
class Koopa;

namespace MR {
    struct ActorMoveParam;
};  // namespace MR

class KoopaStateAttackSpin : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackSpin(Koopa* pKoopa);

    /* 0x08 */ virtual ~KoopaStateAttackSpin();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x14 */ virtual void kill();

    bool attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    bool tryDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWaitToStart();
    void exeRun();
    void exeStart();
    void exeSpin();
    void exeEnd();
    void exeEven();

    /* 0x10 */ s32 mMaxAttacks;
    /* 0x14 */ s32 mAttacks;
    /* 0x18 */ const MR::ActorMoveParam* mMoveParam;
    /* 0x1C */ s32 mSpinDelay;
};
