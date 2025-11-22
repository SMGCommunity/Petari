#pragma once

namespace GameSceneFunction {
    void notifyEndScenarioStarter();
    void requestPlayMovieDemo();
    void requestStartGameOverDemo();
    void requestEndGameOverDemo();
    void requestEndMissDemo();
    void requestPowerStarGetDemo();
    void requestGrandStarGetDemo();
    void requestShowGalaxyMap();
    void requestStaffRoll();
    bool isExecScenarioOpeningCamera();
    bool isExecScenarioStarter();
    bool isExecStageClearDemo();
    void loadScenarioWaveData();
    bool isLoadDoneScenarioWaveData();
    void activateDraw3D();
    void deactivateDraw3D();
};  // namespace GameSceneFunction
