#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;

class GalaxySelectBackButton : public LayoutActor {
public:
    /// @brief Creates a new `GalaxySelectBackButton`.
    GalaxySelectBackButton();

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();

    void disappear();
    void decide();
    bool isPointing() const;
    bool tryPointing();
    bool isAppearing() const;
    bool isDisappearing() const;
    void exeAppear();
    void exeDisappear();
    void exeDecide();

public:
    /* 0x20 */ ButtonPaneController* mPaneCtrl;
};
