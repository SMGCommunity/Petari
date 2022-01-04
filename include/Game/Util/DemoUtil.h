#pragma once

#include "Game/LiveActor/Nerve.h"
class NameObj;
class LiveActor;

namespace MR {
    bool tryRegisterDemoCast(LiveActor *, const JMapInfoIter &);

    bool isDemoCast(const LiveActor *, const char *);

    void registerDemoActionNerve(const LiveActor *, const Nerve *, const char *);

    void registerDemoSimpleCastAll(LiveActor *);

    bool isDemoActive();

    bool isPowerStarGetDemoActive();

    bool tryStartDemo(LiveActor *, const char *);
    void endDemo(NameObj *, const char *);
};