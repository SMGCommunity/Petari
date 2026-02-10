#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class StringSpider : public LiveActor {
public:
    StringSpider(const char*);
    virtual ~StringSpider();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
