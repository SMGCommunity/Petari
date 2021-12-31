#pragma once

#include "Game/LiveActor/Nerve.h"

namespace MR {
    bool tryRegisterDemoCast(LiveActor *, const JMapInfoIter &);

    bool isDemoCast(const LiveActor *, const char *);

    void registerDemoActionNerve(const LiveActor *, const Nerve *, const char *);

    void registerDemoSimpleCastAll(LiveActor *);

    bool isDemoActive();

    bool isPowerStarGetDemoActive();
};