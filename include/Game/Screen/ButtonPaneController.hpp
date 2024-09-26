#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/NerveExecutor.hpp"


class ButtonPaneController : public NerveExecutor {
public:
    ButtonPaneController(LayoutActor* pHost, const char* pPaneName, const char* pBoxButtonName, u32 u, bool b);
    virtual ~ButtonPaneController();
    virtual void update();
    virtual void appear();
    virtual void disappear();
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
    bool startAnimAtFirstStep(const char* pStr);
    bool startPointingAnimAtFirstStep(const char* pStr);
    void setNerveAtAnimStopped(const Nerve* pNerve);
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
    void exeDecidedToDisappear();

    LayoutActor* mHost; // 8
    const char* mPaneName; // 0xC
    const char* mBoxButtonName; // 0x10
    u32 _14;
    bool _18;
    f32 mPointingAnimStartFrame;
    bool _20;
    bool _21;
    bool _22;
    bool _23;
    bool _24;
    const char* mButtonAppearStr; //0x28
    const char* mButtonWaitStr; // 0x2C
    const char* mButtonDecideStr; // 0x30
    const char* mButtonSelectInStr; // 0x34
    const char* mButtonSelectOutStr; // 0x38
    const char* mButtonEndStr; // 0x3C
};

namespace NrvButtonPaneController {
    NERVE(ButtonPaneControllerNrvDisappear);
    NERVE_DECL(ButtonPaneControllerNrvDecidedToDisappear, ButtonPaneController, ButtonPaneController::exeDecidedToDisappear);
    NERVE_DECL_NULL(ButtonPaneControllerNrvDecidedWait);
    NERVE_DECL(ButtonPaneControllerNrvDecided, ButtonPaneController, ButtonPaneController::exeDecided);
    NERVE_DECL(ButtonPaneControllerNrvNotPointing, ButtonPaneController, ButtonPaneController::exeNotPointing);
    NERVE_DECL(ButtonPaneControllerNrvPointing, ButtonPaneController, ButtonPaneController::exePointing);
    NERVE_DECL(ButtonPaneControllerNrvWait, ButtonPaneController, ButtonPaneController::exeWait);
    NERVE_DECL(ButtonPaneControllerNrvAppear, ButtonPaneController, ButtonPaneController::exeAppear);
    NERVE_DECL(ButtonPaneControllerNrvHidden, ButtonPaneController, ButtonPaneController::exeHidden);
};