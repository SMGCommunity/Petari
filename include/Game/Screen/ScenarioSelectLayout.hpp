#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class BackButton;
class CameraContext;
class MultiSceneActor;
class ScenarioSelectStar;

class ScenarioSelectLayout : public LayoutActor {
public:
    ScenarioSelectLayout(EffectSystem*, const CameraContext*);

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();
    virtual void control();

    void calcViewAndEntryStarModel();
    s32 getSelectedScenarioNo() const;
    bool isReadyToDisappear() const;
    void disappear();
    bool isAppearStarEnd() const;
    bool isEndAnimStartStep() const;
    bool isCanceled() const;
    void updateSelectedScenario();
    void updateScenarioText();
    void fadeInText();
    void fadeOutText();
    bool trySelect();
    ScenarioSelectStar* getSelectedStar() const;
    void calcWorldPositionFromScreenPos(TVec3f*, const TVec2f&, f32);
    s32 calcDisplayScenarioNum(s32*, s32*) const;
    void appearAllStar(s32, s32, bool);
    void appearStar(s32, s32, int);
    bool isAppearStarEndAll() const;
    bool tryCancel();
    void setPlayerLeft();
    void startAnimAllNewPane(const char*);
    void setAnimRateAllNewPane(f32);

    void exeAppearStar();
    void exeAppear();
    void exeWaitScenarioSelect();
    void exeDecide();
    void exeAfterScenarioSelected();
    void exeDisappear();
    void exeCancel();
    void exeCancelFadeOut();
    void exeAppearCometWarning();
    void exeWaitCometWarning();
    void exeDisappearCometWarning();

    /* 0x20 */ s32 mSelectedScenarioNo;
    /* 0x24 */ u32 _24;
    /* 0x28 */ bool _28;
    /* 0x2C */ TMtx34f mEffectHostMtx;
    /* 0x5C */ ScenarioSelectStar** mStar;
    /* 0x60 */ MultiSceneActor* mScenarioSky;
    /* 0x64 */ EffectSystem* mEffectSystem;
    /* 0x68 */ const CameraContext* mCameraContext;
    /* 0x6C */ TVec2f mNewTextFollowPos;
    /* 0x74 */ TVec2f mNewGreenTextFollowPos;
    /* 0x7C */ TVec2f mQuestionPane[2];
    /* 0x8C */ TVec2f mStarTopFollowPos;
    /* 0x94 */ BackButton* mBackButton;
    /* 0x98 */ TVec2f mMarioPaneFollowPos;
    /* 0xA0 */ const char* mMarioPaneName;
    /* 0xA4 */ u32 _A4;
    /* 0xA8 */ s32 _A8;
};
