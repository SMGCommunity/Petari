#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class SamboHead : public LiveActor {
public:
    SamboHead(const char*);
    virtual ~SamboHead();

private:
    u8 mPad[(0xB4) - sizeof(LiveActor)];
};
