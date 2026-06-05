#pragma once

#include "Game/Enemy/MogucchiHill.hpp"

class Dodoryu;

class DodoryuHill : public MogucchiHill {
public:
    DodoryuHill(Dodoryu* pHost);

    virtual bool notifyAppearAttackToPlayer(HitSensor*, HitSensor*);

    /* 0xF8 */ Dodoryu* mHost;
};
