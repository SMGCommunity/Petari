#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DharmaSambo : public LiveActor {
public:
    DharmaSambo(const char*);
    /* 0x08 */ virtual ~DharmaSambo();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
