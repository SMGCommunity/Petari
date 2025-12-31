#pragma once

#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Util.hpp"

class LiveActor;
class Nerve;
class DemoExecutor;

class DemoActionInfo {
public:
    DemoActionInfo();

    void registerCast(LiveActor*);
    void registerFunctor(const LiveActor*, const MR::FunctorBase&);
    void registerNerve(const LiveActor*, const Nerve*);

    void executeActionFirst() const;
    void executeActionLast() const;

    const char* mPartName;  // 0x00
    const char* mCastName;  // 0x04
    s32 mCastID;            // 0x08
    s32 mActionType;        // 0x0C
    const char* mPosName;   // 0x10
    const char* mAnimName;  // 0x14
    s32 _18;                      // 0x18
    s32 mCastCount;               // 0x1C
    LiveActor** mCastList;        // 0x20
    MR::FunctorBase** mFunctors;  // 0x24
    const Nerve** mNerves;        // 0x28
    u8 _2C;                       // 0x2C
};

class DemoActionKeeper {
public:
    DemoActionKeeper(const DemoExecutor*);

    void initCast(LiveActor*, const JMapInfoIter&);
    void registerFunctor(const LiveActor*, const MR::FunctorBase&, const char*);
    void registerNerve(const LiveActor*, const Nerve*, const char*);
    void update();
    bool isRegisteredDemoActionAppear(const LiveActor*) const;
    bool isRegisteredDemoActionFunctor(const LiveActor*) const;
    bool isRegisteredDemoActionNerve(const LiveActor*) const;
    bool isRegisteredDemoAction(const LiveActor*, s32) const;

    const DemoExecutor* mDemoExecutor;  // 0x00
    s32 mNumInfos;                      // 0x04
    DemoActionInfo** mInfoArray;        // 0x08
};
