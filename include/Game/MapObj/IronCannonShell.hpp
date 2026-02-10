#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class IronCannonLauncherPoint : public LiveActor {
public:
    IronCannonLauncherPoint(const char*);
    virtual ~IronCannonLauncherPoint();

private:
    u8 mPad[(0x98) - sizeof(LiveActor)];
};

class IronCannonLauncher : public IronCannonLauncherPoint {
public:
    IronCannonLauncher(const char*);
    virtual ~IronCannonLauncher();

private:
    u8 mPad[(0xA4) - sizeof(IronCannonLauncherPoint)];
};
