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
    void requestReset(bool waitForInitialize);
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

    /* 0x14 */ u8 _14;
    /* 0x15 */ u8 _15;
    /* 0x16 */ u8 _16;
    /* 0x17 */ u8 _17;
    /* 0x18 */ ScenarioSelectLayout* mScenarioLayout;
    /* 0x1C */ CinemaFrame* mCinemaFrame;
    /* 0x20 */ J3DDrawBuffer* _20;
    /* 0x24 */ J3DDrawBuffer* _24;
    /* 0x28 */ u8 _28;
    /* 0x29 */ u8 _29;
    /* 0x2A */ u8 _2A;
    /* 0x2B */ u8 _2B;
    /* 0x2C */ EffectSystem* mEffectSystem;
    /* 0x30 */ CameraContext* mCameraContext;
};
