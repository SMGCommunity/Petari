#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;

class GalaxySelectBackButton : public LayoutActor {
public:
    /// @brief Creates a new `GalaxySelectBackButton`.
    GalaxySelectBackButton();

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void control();

    void disappear();
    void decide();
    bool isPointing() const;
    bool tryPointing();
    bool isAppearing() const;
    bool isDisappearing() const;
    void exeAppear();
    void exeDisappear();
    void exeDecide();

private:
    /* 0x20 */ ButtonPaneController* mPaneCtrl;
};
