#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;
class PlayerMissLeft;

class CometRetryButton : public LayoutActor {
public:
    /// @brief Creates a new `CometRetryButton`.
    /// @param pName The pointer to the null-terminated name of the object.
    CometRetryButton(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;
    virtual void appear();
    virtual void kill();
    virtual void control();

    void exeMissLeftWait();
    void exeWait();
    void exeDecided();
    void exeEnd();

public:
    /* 0x20 */ ButtonPaneController* mButtonYesPaneCtrl;
    /* 0x24 */ ButtonPaneController* mButtonNoPaneCtrl;
    /* 0x28 */ PlayerMissLeft* mPlayerMissLeft;
};
