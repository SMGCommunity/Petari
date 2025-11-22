#pragma once

#include "Game/Scene/Scene.hpp"

class GameStageClearSequence;
class GamePauseSequence;
class GameSceneScenarioOpeningCameraState;
class GameScenePauseControl;

class GameScene : public Scene {
public:
    GameScene();

    virtual ~GameScene();
    virtual void init();
    virtual void start();
    virtual void update();
    virtual void draw() const;
    virtual void calcAnim();

    void notifyEndScenarioStarter();
    void requestPlayMovieDemo();
    void requestStartGameOverDemo();
    void requestEndGameOverDemo();
    void requestEndMissDemo();
    void requestPowerStarGetDemo();
    void requestGrandStarGetDemo();
    void setNerveAfterPauseMenu();
    bool isExecScenarioOpeningCamera() const;
    bool isExecScenarioStarter() const;
    bool isExecStageClearDemo() const;
    void exeScenarioOpeningCamera();
    void exeScenarioStarter();
    void exeSceneAction();
    void exePauseMenu();
    void exePowerStarGet();
    void exeGrandStarGet();
    void exeGameOver();
    void exeCometRetryAfterMiss();
    void exeSaveAfterGameOver();
    void exePlayMovie();
    void exeTimeUp();
    void exeGalaxyMap();
    void exeStaffRoll();
    void initSequences();
    void initEffect();
    void drawMirror() const;
    void draw3D() const;
    void draw2D() const;
    bool isValidScenarioOpeningCamera() const NO_INLINE;
    void drawOdhCapture() const;
    void startStagePlayFirst();
    void startStagePlayRetry();
    bool isPermitToPauseMenu() const;
    void requestShowGalaxyMap();
    void requestStaffRoll();
    bool isDrawMirror() const;
    void stageClear();
    inline bool isPlayMovie() const;

    u32 _14;
    GameSceneScenarioOpeningCameraState* mScenarioCamera;  // 0x18
    GameScenePauseControl* mPauseCtrl;                     // 0x1C
    GamePauseSequence* mPauseSeq;                          // 0x20
    GameStageClearSequence* mStageClearSeq;                // 0x24
    bool mDraw3D;                                          // 0x28
    u8 _29;
};
