#pragma once

#include "Game/System/NerveExecutor.hpp"

class LayoutActor;
class Nerve;

class ButtonPaneController : public NerveExecutor {
public:
    /// @brief Creates a new `ButtonPaneController`.
    ButtonPaneController(LayoutActor* pHost, const char* pPaneName, const char* pBoundingPaneName, u32 animIndex, bool);

    void update();
    void appear();
    void disappear();
    bool trySelect();
    bool isHidden() const;
    bool isPointing() const;
    bool isPointingTrigger() const;
    bool isTimingForSelectedSe() const;
    void invalidateDecide();
    void invalidateAppearance();
    bool isAppearing() const;
    bool isDisappearing() const;
    void forceToWait();
    void forceToHide();
    bool isFirstStepWait() const;
    bool isWait() const;
    bool isDecidedWait() const;
    bool onPointing();
    void decide();
    bool isPointingPaneDirect() const;
    bool startAnimAtFirstStep(const char*);
    bool startPointingAnimAtFirstStep(const char*);
    void setNerveAtAnimStopped(const Nerve*);
    bool isPointingPane() const;
    bool tryPointing(bool);
    bool tryNotPointing(bool);
    f32 calcPointingAnimStartFrame() const;
    void exeHidden();
    void exeAppear();
    void exeWait();
    void exePointing();
    void exeNotPointing();
    void exeDecided();
    void exeDecidedWait();
    void exeDecidedToDisappear();
    void exeDisappear();

    /* 0x08 */ LayoutActor* mHost;
    /* 0x0C */ const char* mPaneName;
    /* 0x10 */ const char* mBoundingPaneName;
    /* 0x14 */ u32 mAnimIndex;
    /* 0x18 */ bool _18;
    /* 0x1C */ f32 mPointingAnimStartFrame;
    /* 0x20 */ bool mIsSelected;
    /* 0x21 */ bool mIsPointing;
    /* 0x22 */ bool _22;
    /* 0x23 */ bool _23;
    /* 0x24 */ bool _24;
    /* 0x28 */ const char* mAppearAnimName;
    /* 0x2C */ const char* mWaitAnimName;
    /* 0x30 */ const char* mDecideAnimName;
    /* 0x34 */ const char* mPointingAnimName;
    /* 0x38 */ const char* mNotPointingAnimName;
    /* 0x3C */ const char* mDisappearAnimName;
};
