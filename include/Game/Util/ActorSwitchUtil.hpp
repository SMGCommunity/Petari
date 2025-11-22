#pragma once

class LiveActor;
class JMapInfoIter;

namespace MR {
    class FunctorBase;

    bool useStageSwitchReadA(LiveActor*, const JMapInfoIter&);
    bool useStageSwitchReadB(LiveActor*, const JMapInfoIter&);
    bool useStageSwitchReadAppear(LiveActor*, const JMapInfoIter&);
    void useStageSwitchSleep(LiveActor*, const JMapInfoIter&);
    bool useStageSwitchWriteA(LiveActor*, const JMapInfoIter&);
    bool useStageSwitchWriteB(LiveActor*, const JMapInfoIter&);
    bool useStageSwitchWriteDead(LiveActor*, const JMapInfoIter&);
    bool needStageSwitchReadA(LiveActor*, const JMapInfoIter&);
    bool needStageSwitchReadB(LiveActor*, const JMapInfoIter&);
    bool needStageSwitchReadAppear(LiveActor*, const JMapInfoIter&);
    bool needStageSwitchWriteA(LiveActor*, const JMapInfoIter&);
    bool needStageSwitchWriteB(LiveActor*, const JMapInfoIter&);
    bool needStageSwitchWriteDead(LiveActor*, const JMapInfoIter&);
    bool isValidSwitchA(const LiveActor*);
    bool isValidSwitchB(const LiveActor*);
    bool isValidSwitchAppear(const LiveActor*);
    bool isValidSwitchDead(const LiveActor*);
    bool isOnSwitchA(const LiveActor*);
    bool isOnSwitchB(const LiveActor*);
    bool isOnSwitchAppear(const LiveActor*);
    void onSwitchA(LiveActor*);
    void onSwitchB(LiveActor*);
    void onSwitchDead(LiveActor*);
    void offSwitchA(LiveActor*);
    void offSwitchB(LiveActor*);
    void offSwitchDead(LiveActor*);
    void syncStageSwitchAppear(LiveActor*);
    void listenStageSwitchOnAppear(LiveActor*, const MR::FunctorBase&);
    void listenStageSwitchOnOffAppear(LiveActor*, const MR::FunctorBase&, const MR::FunctorBase&);
    void listenStageSwitchOnA(LiveActor*, const MR::FunctorBase&);
    void listenStageSwitchOnOffA(LiveActor*, const MR::FunctorBase&, const MR::FunctorBase&);
    void listenStageSwitchOnB(LiveActor*, const MR::FunctorBase&);
    void listenStageSwitchOffB(LiveActor*, const MR::FunctorBase&);
    void listenStageSwitchOnOffB(LiveActor*, const MR::FunctorBase&, const MR::FunctorBase&);
};  // namespace MR
