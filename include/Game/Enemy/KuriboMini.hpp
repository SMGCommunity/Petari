#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class KuriboMini : public LiveActor {
public:
    KuriboMini(const char*);
    virtual ~KuriboMini();

private:
    u8 mPad[(0xC4) - sizeof(LiveActor)];
};
