#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;
class LuigiLetter;
class SysInfoWindow;

class PauseMenu : public LayoutActor {
public:
    /// @brief Creates a new `PauseMenu`.
    PauseMenu();

    /// @brief Intializes the `PauseMenu` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    /// @brief Draws the `PauseMenu` to the screen.
    /* 0x18 */ virtual void draw() const;

    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();
    /* 0x2C */ virtual void control();

    void updateStarPane();
    void startPaneAnimWithoutButton(const char*);
    bool isPaneAnimStoppedWithoutButton() const;
    void forceToWaitAllButton();
    void exeSelecting();
    void exeDecided();
    void exeDisappear();
    void exeConfirm();
    void exeGameDataSave();
    void exeLuigiLetter();

    /* 0x20 */ ButtonPaneController* _20;
    /* 0x24 */ ButtonPaneController* _24;
    /* 0x28 */ TVec2f _28;
    /* 0x30 */ SysInfoWindow* _30;
    /* 0x34 */ LuigiLetter* mLuigiLetter;
    /* 0x38 */ ButtonPaneController* _38;
    /* 0x3C */ TVec2f mStageTitleOffset;
    /* 0x44 */ TVec2f mCoinNumPos;
    /* 0x4C */ TVec2f mStarPieceNumPos;
    /* 0x54 */ bool _54;
};
