#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TakoHei : public LiveActor {
public:
    TakoHei(const char*);
    virtual ~TakoHei();

private:
    u8 mPad[(0xF0) - sizeof(LiveActor)];
};
