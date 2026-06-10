#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class KoteBug : public LiveActor {
public:
    KoteBug(const char*);
    virtual ~KoteBug();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
