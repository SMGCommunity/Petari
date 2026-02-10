#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class TrickRabbitSnowCollect : public LiveActor {
public:
    TrickRabbitSnowCollect(const char*);
    virtual ~TrickRabbitSnowCollect();

private:
    u8 mPad[(0xDC) - sizeof(LiveActor)];
};
