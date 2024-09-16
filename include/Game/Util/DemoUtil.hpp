#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/Functor.hpp"

class NameObj;
class LiveActor;
class JMapInfoIter;

namespace MR {
    bool tryRegisterDemoCast(LiveActor *, const char *, const JMapInfoIter &);
    bool tryRegisterDemoCast(LiveActor *, const JMapInfoIter &);

    bool isDemoCast(const LiveActor *, const char *);

    void registerDemoActionFunctor(const LiveActor *, const MR::FunctorBase &, const char *);
    bool tryRegisterDemoActionFunctorDirect(const LiveActor *, const MR::FunctorBase &, const char *, const char *);

    void registerDemoActionNerve(const LiveActor *, const Nerve *, const char *);
    bool tryRegisterDemoActionFunctor(const LiveActor *, const MR::FunctorBase &, const char *);

    bool tryRegisterDemoActionNerve(const LiveActor *, const Nerve *, const char *);

    bool isRegisteredDemoActionNerve(const LiveActor *);

    void registerDemoSimpleCastAll(LiveActor *);
    void registerDemoSimpleCastAll(NameObj *);

    bool isDemoActive();
    bool isDemoActive(const char *);

    bool isDemoExist(const char *);

    bool isPowerStarGetDemoActive();

    const char* getCurrentDemoPartNameMain(const char *);

    bool isTimeKeepDemoActive();

    bool tryStartDemo(LiveActor *, const char *);
    bool tryStartDemoWithoutCinemaFrame(LiveActor *, const char *);
    bool tryStartDemoRegistered(LiveActor *, const char *);
    void endDemo(NameObj *, const char *);

    bool requestStartDemo(LiveActor *, const char *, const Nerve *, const Nerve *);

    void requestStartDemoMarioPuppetableWithoutCinmeaFrame(LiveActor *, const char *, const Nerve *, const Nerve *);
    void requestStartDemoWithoutCinemaFrame(LiveActor *, const char *, const Nerve *, const Nerve *);

    bool tryStartDemoMarioPuppetable(LiveActor *, const char *);

    bool tryStartTimeKeepDemoMarioPuppetable(LiveActor *, const char *, const char *);
    bool tryStartTimeKeepDemoMarioPuppetable(NameObj *, const char *, const char *);

    void requestStartTimeKeepDemoMarioPuppetable(LiveActor *, const char *, const Nerve *, const Nerve *, const char *);

    void requestStartTimeKeepDemo(LiveActor *, const char *, const Nerve *, const Nerve *, const char *);

    bool isDemoPartStep(const char *, s32);
    bool isDemoPartFirstStep(const char *);
    bool isDemoPartLastStep(const char *);

    s32 getDemoPartTotalStep(const char *);

    bool isDemoPartTalk(const char *);

    void endDemoWaitCameraInterpolating(NameObj *, const char *);

    bool isSystemTalking();
    bool isNormalTalking();
};