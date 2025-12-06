#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"
#include "Game/Util/JMapInfo.hpp"

class DemoDirector;
class DemoCastGroupHolder;
class DemoExecutor;

namespace DemoFunction {
    DemoDirector* getDemoDirector();
    DemoCastGroupHolder* getDemoCastSubGroupHolder();

    void registerDemoSimpleCastAllFunction(LiveActor*);
    void registerDemoSimpleCastAllFunction(LayoutActor*);
    void registerDemoSimpleCastAllFunction(NameObj*);
    void registerDemoActionFunctorFunction(const LiveActor*, const MR::FunctorBase&, const char*);
    void registerDemoActionFunctorFunction(const LiveActor*, const MR::FunctorBase&, const char*, const char*);
    void registerDemoActionNerveFunction(const LiveActor*, const Nerve*, const char*);

    bool tryCreateDemoTalkAnimCtrlForActor(LiveActor*, const char*, const char*);
    bool tryCreateDemoTalkAnimCtrlForActorDirect(LiveActor*, const char*, const char*, const char*);

    bool tryCreateDemoTalkAnimCtrlForScene(LiveActor*, const JMapInfoIter&, const char*, const char*, s32, s32);

    bool tryStartDemoRegistered(LiveActor*, const char*);
    bool tryStartDemoRegisteredMarioPuppetable(LiveActor*, const char*);

    void registerDemoExecutor(DemoExecutor*);
    DemoExecutor* findDemoExecutor(const char*);
    DemoExecutor* findDemoExecutor(const LiveActor*);
    DemoExecutor* findDemoExecutorActive(const LiveActor*);

    bool isDemoCast(const DemoExecutor*, const LiveActor*);
    bool isExistDemoPart(const DemoExecutor*, const char*);

    bool isDemoPartActiveFunction(const char*);
    s32 getDemoPartStepFunction(const char*);
    s32 getDemoPartTotalStepFunction(const char*);
    bool isDemoLastPartLastStep();
    bool isDemoSuspendOrLastPartLastStep();
    const char* getCurrentDemoPartNameMain(const char*);

    bool isTargetDemoCast(LiveActor*, const JMapInfoIter&, const char*, s32);

    bool isRegisteredDemoActionAppear(const LiveActor*);
    bool isRegisteredDemoActionFunctor(const LiveActor*);
    bool isRegisteredDemoActionFunctor(const LiveActor*, const char*);
    bool isRegisteredDemoActionNerve(const LiveActor*);

    void requestDemoCastMovementOn(LiveActor*);
    bool isRegisteredDemoCast(const LiveActor*, const char*);

    void pauseTimeKeepDemo(LiveActor*);
    void resumeTimeKeepDemo(LiveActor*);
    bool isPauseTimeKeepDemo(LiveActor*);

    void pauseTimeKeepDemo(const char*);
    void resumeTimeKeepDemo(const char*);
    bool isPauseTimeKeepDemo(const char*);

    bool tryCreateDemoTalkAnimCtrlForSceneDirect(LiveActor*, const char*, const JMapInfoIter&, const char*, const char*, s32, s32);
    void registerDemoTalkMessageCtrl(LiveActor*, TalkMessageCtrl*);
    void registerDemoTalkMessageCtrlDirect(LiveActor*, TalkMessageCtrl*, const char*);
    void setDemoTalkMessageCtrlDirect(const LiveActor*, TalkMessageCtrl*, const char*);

    ResourceHolder* loadDemoArchive();

    s32 createSheetParser(const DemoExecutor*, const char*, JMapInfo**);

    bool isDemoPartTalk(const char*);
};  // namespace DemoFunction
