#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class RunawayRabbitCollect : public LiveActor {
public:
    RunawayRabbitCollect(const char*);
    virtual ~RunawayRabbitCollect();

private:
    u8 mPad[(0xB4) - sizeof(LiveActor)];
};
