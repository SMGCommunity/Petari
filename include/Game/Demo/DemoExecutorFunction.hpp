#pragma once

#include "Game/Demo/DemoExecutor.hpp"

class LiveActor;
class TalkMessageCtrl;

namespace DemoExecutorFunction {
    void registerDemoActionFunction(const DemoExecutor*, const LiveActor*, const MR::FunctorBase&, const char*);
    void registerDemoActionNerve(const DemoExecutor*, const LiveActor*, const Nerve*, const char*);

    bool isExistDemoPart(const DemoExecutor*, const char*);
    bool isExistDemoPartMain(const DemoExecutor*, const char*);
    bool isExistDemoPartSub(const DemoExecutor*, const char*);

    bool isRegisteredDemoCast(const DemoExecutor*, const LiveActor*);
    bool isRegisteredDemoActionAppear(const DemoExecutor*, const LiveActor*);
    bool isRegisteredDemoActionFunctor(const DemoExecutor*, const LiveActor*);
    bool isRegisteredDemoActionNerve(const DemoExecutor*, const LiveActor*);

    bool tryCreateDemoTalkAnimCtrlForActor(DemoExecutor*, LiveActor*, const char*, const char*);
    bool tryCreateDemoTalkAnimCtrlForScene(DemoExecutor*, LiveActor*, const JMapInfoIter&, const char*, const char*, s32, s32);

    void registerDemoTalkMessageCtrl(DemoExecutor*, LiveActor*, TalkMessageCtrl*);
    void setDemoTalkMessageCtrl(DemoExecutor*, const LiveActor*, TalkMessageCtrl*);

    void pauseTimeKeepDemo(DemoExecutor*);
    void resumeTimeKeepDemo(DemoExecutor*);
    bool isPauseTimeKeepDemo(const DemoExecutor*);
};  // namespace DemoExecutorFunction
