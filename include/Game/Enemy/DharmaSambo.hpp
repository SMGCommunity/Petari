#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class DharmaSambo : public LiveActor {
public:
    DharmaSambo(const char*);
    virtual ~DharmaSambo();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
