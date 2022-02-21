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

    bool tryRegisterDemoActionNerve(const LiveActor *, const Nerve *, const char *);

    bool isRegisteredDemoActionNerve(const LiveActor *);

    void registerDemoSimpleCastAll(LiveActor *);

    bool isDemoActive();

    bool isPowerStarGetDemoActive();

    bool tryStartDemo(LiveActor *, const char *);
    bool tryStartDemoRegistered(LiveActor *, const char *);
    void endDemo(NameObj *, const char *);

    void requestStartDemoMarioPuppetableWithoutCinmeaFrame(LiveActor *, const char *, const Nerve *, const Nerve *);
    void requestStartDemoWithoutCinemaFrame(LiveActor *, const char *, const Nerve *, const Nerve *);
};