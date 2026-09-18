#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;

/// @brief The actor for the letter from Princess Peach during the beginning of the game.
class PrologueLetter : public LayoutActor {
public:
    /// @brief Creates a new `PrologueLetter`.
    /// @param pName The pointer to the null-terminated name of the object.
    PrologueLetter(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();

    void pauseOff();

    /// @brief Updates the object when transitioning on screen.
    void exeAppear();

    /// @brief Updates the object when waiting for player input.
    void exeWait();

    /// @brief Updates the object when transitioning off screen.
    void exeDisappear();

public:
    /// @brief The pointer to the actor instance for the A Button.
    /* 0x20 */ IconAButton* mAButtonIcon;
};
