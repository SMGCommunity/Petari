#pragma once

namespace MR {
    enum MovementType {

    };

    enum CalcAnimType {

    };

    enum DrawType {

    };
};

class CategoryList {
public:
    static void startStageFileLoad();
    static void waitDoneStageFileLoad();
    static void startActorFileLoadCommon();
    static void startActorFileLoadScenario();
    static void startActorPlacement();
    static void initAfterScenarioSelected();
    static void initForNameObj();
    static void initForLiveActor();
    static void initEffectSystem();
    static void allocateDrawBufferActorList();
    static void execute(MR::MovementType);
    static void execute(MR::CalcAnimType);
    static void execute(MR::DrawType);
    static void entryDrawBuffer2D();
    static void entryDrawBuffer3D();
    static void entryDrawBufferMirror();
};