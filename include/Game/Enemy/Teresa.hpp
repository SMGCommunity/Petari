#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Teresa : public LiveActor {
public:
    Teresa(const char*);
    virtual ~Teresa();

private:
    u8 mPad[(0x100) - sizeof(LiveActor)];
};
