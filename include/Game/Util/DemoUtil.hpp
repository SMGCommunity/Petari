#pragma once

#include "Game/LiveActor/Nerve.h"
#include "Game/Util/Functor.h"
class NameObj;
class LiveActor;

namespace MR {
    bool tryRegisterDemoCast(LiveActor *, const JMapInfoIter &);

    bool isDemoCast(const LiveActor *, const char *);

    void registerDemoActionFunctor(const LiveActor *, const MR::FunctorBase &, const char *);

    void registerDemoActionNerve(const LiveActor *, const Nerve *, const char *);
    bool tryRegisterDemoActionFunctor(const LiveActor *, const MR::FunctorBase &, const char *);

    bool tryRegisterDemoActionNerve(const LiveActor *, const Nerve *, const char *);

    bool isRegisteredDemoActionNerve(const LiveActor *);

    void registerDemoSimpleCastAll(LiveActor *);
    void registerDemoSimpleCastAll(NameObj *);

    bool isDemoActive();

    bool isDemoExist(const char *);

    bool isPowerStarGetDemoActive();

    bool isTimeKeepDemoActive();

    bool tryStartDemo(LiveActor *, const char *);
    bool tryStartDemoWithoutCinemaFrame(LiveActor *, const char *);
    bool tryStartDemoRegistered(LiveActor *, const char *);
    void endDemo(NameObj *, const char *);

    bool requestStartDemo(LiveActor *, const char *, const Nerve *, const Nerve *);

    void requestStartDemoMarioPuppetableWithoutCinmeaFrame(LiveActor *, const char *, const Nerve *, const Nerve *);
    void requestStartDemoWithoutCinemaFrame(LiveActor *, const char *, const Nerve *, const Nerve *);

    void requestStartTimeKeepDemo(LiveActor *, const char *, const Nerve *, const Nerve *, const char *);

    bool isDemoPartLastStep(const char *);

    void endDemoWaitCameraInterpolating(NameObj *, const char *);
};