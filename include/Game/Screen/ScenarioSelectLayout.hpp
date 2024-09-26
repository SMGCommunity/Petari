#pragma once

#include "Game/Screen/BackButton.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Camera/CameraContext.hpp"
#include "Game/Map/ScenarioSelectStar.hpp"

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
    bool isAppearStarEnd() const;
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

    s32 mSelectedScenarioNo;                // 0x24
    u32 _24;
    u8 _28;
    TMtx34f mEffectHostMtx;                 // 0x2C
    ScenarioSelectStar** mStars;            // 0x5C
    MultiSceneActor* mScenarioSky;          // 0x60
    EffectSystem* mEffectSystem;            // 0x64
    const CameraContext* mCameraContext;    // 0x68
    TVec2f mNewTextFollowPos;               // 0x6C
    TVec2f mNewGreenTextFollowPos;          // 0x74
    TVec2f mQuestionPane[2];                // 0x7C
    TVec2f mStarTopFollowPos;               // 0x8C
    BackButton* mBackButton;                // 0x94
    TVec2f mMarioIconFollowPos;             // 0x98
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