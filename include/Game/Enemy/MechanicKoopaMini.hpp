#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class MechanicKoopaMini : public LiveActor {
public:
    MechanicKoopaMini(const char*);
    virtual ~MechanicKoopaMini();

private:
    u8 mPad[(0x98) - sizeof(LiveActor)];
};
