#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class UnizoLauncher : public LiveActor {
public:
    UnizoLauncher(const char*);
    /* 0x08 */ virtual ~UnizoLauncher();

private:
    u8 mPad[(0x94) - sizeof(LiveActor)];
};
