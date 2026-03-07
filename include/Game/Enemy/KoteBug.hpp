#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class KoteBug : public LiveActor {
public:
    KoteBug(const char*);
    virtual ~KoteBug();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
