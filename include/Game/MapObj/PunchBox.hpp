#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class PunchBox : public LiveActor {
public:
    PunchBox(const char*);
    virtual ~PunchBox();

private:
    u8 mPad[(0xA0) - sizeof(LiveActor)];
};
