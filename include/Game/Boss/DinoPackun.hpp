#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DinoPackun : public LiveActor {
public:
    void attackSensorTail(HitSensor*, HitSensor*);

    bool receiveMsgPlayerAttackTail(u32, HitSensor*, HitSensor*);
};
