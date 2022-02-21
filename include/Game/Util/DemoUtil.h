#pragma once

#include "Game/LiveActor/Nerve.h"
#include "Game/Util/Functor.h"
class NameObj;
class LiveActor;

namespace MR {
    bool tryRegisterDemoCast(LiveActor *, const JMapInfoIter &);

    bool isDemoCast(const LiveActor *, const char *);

    void registerDemoActionNerve(const LiveActor *, const Nerve *, const char *);
    bool tryRegisterDemoActionFunctor(const LiveActor *, const MR::FunctorBase &, const char *);

    void registerDemoSimpleCastAll(LiveActor *);
    void registerDemoSimpleCastAll(NameObj *);

    bool isDemoActive();

    bool isPowerStarGetDemoActive();

    bool tryStartDemo(LiveActor *, const char *);
    bool tryStartDemoRegistered(LiveActor *, const char *);
    void endDemo(NameObj *, const char *);
};