#pragma once

#include "LiveActor/LiveActor.h"
#include "JMap/JMapInfoIter.h"
#include "Util/FunctorBase.h"

namespace MR
{
    bool useStageSwitchReadA(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchReadB(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchReadAppear(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchSleep(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchWriteA(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchWriteB(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchWriteDead(LiveActor *, const JMapInfoIter &);

    bool isValidSwitchA(const LiveActor *);
    bool isValidSwitchB(const LiveActor *);
    bool isValidSwitchAppear(const LiveActor *);
    bool isValidSwitchDead(const LiveActor *);
    bool isOnSwitchA(const LiveActor *);
    bool isOnSwitchB(const LiveActor *);
    bool isOnSwitchAppear(const LiveActor *);
    void onSwitchA(LiveActor *);
    void onSwitchB(LiveActor *);
    void onSwitchDead(LiveActor *);
    void offSwitchA(LiveActor *);
    void offSwitchB(LiveActor *);
    void offSwitchDead(LiveActor *);

    void listenStageSwitchOnA(LiveActor *, const MR::FunctorBase &);
};