#pragma once

#include "Game/Screen/LayoutActor.hpp"

class BackButton;
class ButtonPaneController;

class GalaxyConfirmLayout : public LayoutActor {
public:
    /// @brief Creates a new `GalaxyConfirmLayout`.
    GalaxyConfirmLayout();

    /// @brief Destroys the `GalaxyConfirmLayout`.
    virtual ~GalaxyConfirmLayout() {}

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    bool isSelected() const;
    bool isSelectedYes() const;
    void requestMovementOn();
    void exeSelecting();
    void exeSelectedBackButton();
    void exeSelected();

private:
    /* 0x20 */ ButtonPaneController* mPaneCtrl;
    /* 0x24 */ BackButton* mBackButton;
};
