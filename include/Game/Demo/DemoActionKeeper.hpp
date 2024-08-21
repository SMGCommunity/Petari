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
    s32 mCastCount;                 // 0x1C
    LiveActor** mCastList;          // 0x20
    MR::FunctorBase** mFunctors;    // 0x24
    const Nerve** mNerves;          // 0x28
};
