#pragma once

#include "Game/Util.hpp"

class LiveActor;
class Nerve;

class DemoActionInfo {
public:
    DemoActionInfo();

    void registerCast(LiveActor *);
    void registerFunctor(const LiveActor *, const MR::FunctorBase &);
    void registerNerve(const LiveActor *, const Nerve *);

    u8 _0[0x1C];
    s32 mCastCount;                 // _1C
    LiveActor** mCastList;          // _20
    MR::FunctorBase** mFunctors;    // _24
    const Nerve** mNerves;          // _28
};
