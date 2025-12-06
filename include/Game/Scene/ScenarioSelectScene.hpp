#pragma once

#include "Game/Scene/Scene.hpp"

class CameraContext;
class CinemaFrame;
class EffectSystem;
class J3DDrawBuffer;
class ScenarioSelectLayout;

class ScenarioSelectScene : public Scene {
public:
    ScenarioSelectScene();

    virtual ~ScenarioSelectScene();
    virtual void init();
    virtual void start();
    virtual void update();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void calcViewAndEntry();

    void startBackground();
    bool isActive() const;
    bool isExecForeground() const;
    bool isScenarioSelecting() const;
    void validateScenarioSelect();
    void requestReset(bool);
    bool isResetEnd() const;
    void setupCameraMtx() const;
    bool trySetCurrentScenarioNo() const;
    void suspend();
    void tryStartScreenToFrame();
    void exeDeactive();
    void exeInvalidScenarioSelect();
    void exeWaitStartScenarioSelect();
    void exeStartScenarioSelect();
    void exeWaitScenarioSelect();
    void exeWaitResumeInitializeThread() NO_INLINE;
    void exeWaitInitializeEnd();
    void exeWaitDisappearLayout();
    void exeWaitResumeInitializeThreadIfRequestedReset();
    void exeWaitResumeInitializeThreadIfCanceledSelect();

    u8 _14;
    u8 _15;
    u8 _16;
    u8 _17;
    ScenarioSelectLayout* mScenarioLayout;  // 0x18
    CinemaFrame* mCinemaFrame;              // 0x1C
    J3DDrawBuffer* _20;
    J3DDrawBuffer* _24;
    u8 _28;
    u8 _29;
    u8 _2A;
    u8 _2B;
    EffectSystem* mEffectSystem;    // 0x2C
    CameraContext* mCameraContext;  // 0x30
};
