#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class RunawayRabbitCollect : public LiveActor {
public:
    RunawayRabbitCollect(const char*);
    virtual ~RunawayRabbitCollect();

private:
    u8 mPad[(0xB4) - sizeof(LiveActor)];
};
