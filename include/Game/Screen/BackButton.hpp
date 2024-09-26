#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;

class BackButton : public LayoutActor {
public:
    BackButton(const char *, bool);

    virtual ~BackButton();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void control();

    void disappear();
    bool isHidden() const;

    ButtonPaneController* mController;      // 0x20
    u8 _24;
    u8 _25;
};