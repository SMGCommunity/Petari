#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class DinoPackun;
class HitSensor;

class DinoPackunStateDamage : public ActorStateBase< DinoPackun > {
public:
    DinoPackunStateDamage(DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunStateDamage();
    /* 0x10 */ virtual void appear();

    bool isDamageMessage(u32) const;
    bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    void setDamageEgg();
    void setDamageNormal();
    void setDamageLast();
    void exePunched();
    void exePulled();
    void exeTryPulledDemo();
    void exeEggBroken();
    void exeDamage();
    void exeLastDamage();
    void updateDamage();

    u32 _10;
    u32 _14;
};
