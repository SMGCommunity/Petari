#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;

/// @brief The actor for the window encouraging the player to change their TV Type from 50 Hz to 60 Hz.
class EncouragePal60Window : public LayoutActor {
public:
    /// @brief Creates a new `EncouragePal60Window`.
    EncouragePal60Window();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    /// @brief Updates the object when transitioning on screen.
    void exeAppear();

    /// @brief Updates the object when idle or waiting for player input.
    void exeDisplay();

    /// @brief Updates the object when transitioning off screen.
    void exeDisappear();

public:
    /// @brief The pointer to the actor instance for the A Button.
    /* 0x20 */ IconAButton* mAButtonIcon;
};
