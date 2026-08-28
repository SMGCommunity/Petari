#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RunawayRabbitCollect : public LiveActor {
public:
    RunawayRabbitCollect(const char*);
    /* 0x08 */ virtual ~RunawayRabbitCollect();

private:
    u8 mPad[(0xB4) - sizeof(LiveActor)];
};
