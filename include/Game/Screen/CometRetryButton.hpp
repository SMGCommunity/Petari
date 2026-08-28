#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;
class PlayerMissLeft;

class CometRetryButton : public LayoutActor {
public:
    /// @brief Creates a new `CometRetryButton`.
    /// @param pName The pointer to the null-terminated name of the object.
    CometRetryButton(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x18 */ virtual void draw() const;
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();
    /* 0x2C */ virtual void control();

    void exeMissLeftWait();
    void exeWait();
    void exeDecided();
    void exeEnd();

private:
    /* 0x20 */ ButtonPaneController* mButtonYesPaneCtrl;
    /* 0x24 */ ButtonPaneController* mButtonNoPaneCtrl;
    /* 0x28 */ PlayerMissLeft* mPlayerMissLeft;
};
