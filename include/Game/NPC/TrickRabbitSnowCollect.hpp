#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class TrickRabbitSnowCollect : public LiveActor {
public:
    TrickRabbitSnowCollect(const char*);
    virtual ~TrickRabbitSnowCollect();

private:
    u8 mPad[(0xDC) - sizeof(LiveActor)];
};
