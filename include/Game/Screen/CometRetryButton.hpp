#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;
class PlayerMissLeft;

class CometRetryButton : public LayoutActor {
public:
    CometRetryButton(const char *);

    virtual ~CometRetryButton();
    virtual void init(const JMapInfoIter &);
    virtual void draw() const;
    virtual void appear();
    virtual void kill();
    virtual void control();

    ButtonPaneController* mUpButton;        // 0x20
    ButtonPaneController* mDownButton;      // 0x24
    PlayerMissLeft* mPlayerMissLeft;        // 0x28
};
