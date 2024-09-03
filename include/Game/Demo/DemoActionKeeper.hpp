#pragma once

#include "Game/Util.hpp"
#include "Game/Demo/DemoExecutor.hpp"

class LiveActor;
class Nerve;
class DemoExecutor;

class DemoActionInfo {
public:
    DemoActionInfo();

    void registerCast(LiveActor *);
    void registerFunctor(const LiveActor *, const MR::FunctorBase &);
    void registerNerve(const LiveActor *, const Nerve *);

    u8 _0[0x1C];
    s32 mCastCount;                 // 0x1C
    LiveActor** mCastList;          // 0x20
    MR::FunctorBase** mFunctors;    // 0x24
    const Nerve** mNerves;          // 0x28
};

class DemoActionKeeper {
public:
    void initCast(LiveActor *, const JMapInfoIter &);
    void registerFunctor(const LiveActor *, const MR::FunctorBase &, const char *);
    void registerNerve(const LiveActor *, const Nerve *, const char *);
    void update();
    bool isRegisteredDemoActionAppear(const LiveActor *) const;
    bool isRegisteredDemoActionFunctor(const LiveActor *) const;
    bool isRegisteredDemoActionNerve(const LiveActor *) const;
    bool isRegisteredDemoAction(const LiveActor *, long) const;

    DemoActionKeeper(const DemoExecutor *);
};
