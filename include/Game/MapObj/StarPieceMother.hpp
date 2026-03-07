#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class StarPieceMother : public LiveActor {
public:
    StarPieceMother(const char*);
    virtual ~StarPieceMother();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
