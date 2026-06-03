#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class DharmaSambo : public LiveActor {
public:
    DharmaSambo(const char*);
    virtual ~DharmaSambo();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
