#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class DinoPackun;
class HitSensor;

class DinoPackunStateDamage : public ActorStateBase< DinoPackun > {
public:
    DinoPackunStateDamage(DinoPackun*);

    virtual ~DinoPackunStateDamage();
    virtual void appear();

    bool isDamageMessage(u32) const;
    bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    void setDamageEgg();
    void setDamageNormal();
    void setDamageLast();
    void exePunched();
    void exePulled();
    void exeEggBroken();
    void exeDamage();
    void exeLastDamage();
    void updateDamage();

    u32 _10;
    u32 _14;
};
