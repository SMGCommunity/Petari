#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class MechanicKoopaMini : public LiveActor {
public:
    MechanicKoopaMini(const char*);
    virtual ~MechanicKoopaMini();

private:
    u8 mPad[(0x98) - sizeof(LiveActor)];
};
