#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class KoopaBattleMapCoinPlate : public LiveActor {
public:
    KoopaBattleMapCoinPlate(const char*);
    virtual ~KoopaBattleMapCoinPlate();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
