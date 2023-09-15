#pragma once

#include "Game/Screen/BackButton.h"
#include "Game/Screen/LayoutActor.h"
#include "Game/Camera/CameraContext.h"
#include "Game/Map/ScenarioSelectStar.h"

class ScenarioSelectLayout : public LayoutActor {
public:
    ScenarioSelectLayout(EffectSystem *, const CameraContext *);

    virtual ~ScenarioSelectLayout();
    virtual void init(const JMapInfoIter &);
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
    bool isEndAnimStartStep() const;
    bool isCanceled() const;
    void updateSelectedScenario();
    void updateScenarioText();
    void fadeInText();
    void fadeOutText();
    bool trySelect();
    ScenarioSelectStar* getSelectedStar() const;
    void calcWorldPositionFromScreenPos(TVec3f *, const TVec2f &, f32);
    s32 calcDisplayScenarioNum(s32 *, s32 *) const;
    void appearAllStar(s32, s32, bool);
    void appearStar(s32, s32, int);
    bool isAppearStarEndAll() const;
    bool tryCancel();
    void setPlayerLeft();
    void startAnimAllNewPane(const char *);
    void setAnimRateAllNewPane(f32);

    void exeAppearStar();
    void exeAppear();
    void exeWaitScenarioSelect();
    void exeDecide();
    void exeAfterScenarioSelected();
    void exeDisappear();
    void exeCancel();
    void exeAppearCometWarning();
    void exeWaitCometWarning();

    void exeDisappearCometWarning();
    inline void exeCancelFadeOut();

    s32 mSelectedScenarioNo;                // _24
    u32 _24;
    u8 _28;
    TMtx34f mEffectHostMtx;                 // _2C
    ScenarioSelectStar** mStars;            // _5C
    MultiSceneActor* mScenarioSky;          // _60
    EffectSystem* mEffectSystem;            // _64
    const CameraContext* mCameraContext;    // _68
    TVec2f mNewTextFollowPos;               // _6C
    TVec2f mNewGreenTextFollowPos;          // _74
    TVec2f mQuestionPane[2];                // _7C
    TVec2f mStarTopFollowPos;               // _8C
    BackButton* mBackButton;                // _94
    TVec2f mMarioIconFollowPos;             // _98
    const char* _A0;
    u32 _A4;
    s32 _A8;
};

namespace NrvScenarioSelectLayout {
    NERVE_DECL(ScenarioSelectLayoutNrvAppearStar, ScenarioSelectLayout, ScenarioSelectLayout::exeAppearStar)
    NERVE_DECL(ScenarioSelectLayoutNrvAppear, ScenarioSelectLayout, ScenarioSelectLayout::exeAppear)
    NERVE_DECL(ScenarioSelectLayoutNrvWaitScenarioSelect, ScenarioSelectLayout, ScenarioSelectLayout::exeWaitScenarioSelect)
    NERVE_DECL(ScenarioSelectLayoutNrvDecide, ScenarioSelectLayout, ScenarioSelectLayout::exeDecide)
    NERVE_DECL(ScenarioSelectLayoutNrvAfterScenarioSelected, ScenarioSelectLayout, ScenarioSelectLayout::exeAfterScenarioSelected)
    NERVE_DECL(ScenarioSelectLayoutNrvDisappear, ScenarioSelectLayout, ScenarioSelectLayout::exeDisappear)
    NERVE_DECL(ScenarioSelectLayoutNrvCancel, ScenarioSelectLayout, ScenarioSelectLayout::exeCancel)
    NERVE_DECL(ScenarioSelectLayoutNrvCancelFadeOut, ScenarioSelectLayout, ScenarioSelectLayout::exeCancelFadeOut)
    NERVE_DECL(ScenarioSelectLayoutNrvAppearCometWarning, ScenarioSelectLayout, ScenarioSelectLayout::exeAppearCometWarning)
    NERVE_DECL(ScenarioSelectLayoutNrvWaitCometWarning, ScenarioSelectLayout, ScenarioSelectLayout::exeWaitCometWarning)
    NERVE_DECL(ScenarioSelectLayoutNrvDisappearCometWarning, ScenarioSelectLayout, ScenarioSelectLayout::exeDisappearCometWarning)
};