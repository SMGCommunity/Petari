#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class UnizoLauncher : public LiveActor {
public:
    UnizoLauncher(const char*);
    virtual ~UnizoLauncher();

private:
    u8 mPad[(0x94) - sizeof(LiveActor)];
};
