#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;

class BackButton : public LayoutActor {
public:
    BackButton(const char* pName, bool param2);

    /* 0x08 */ virtual ~BackButton();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void control();

    void disappear();
    bool isHidden() const;
    bool isAppearing() const;
    bool isDisappearing() const;
    bool isPointing() const;

    /* 0x20 */ ButtonPaneController* mPaneCtrl;
    /* 0x24 */ bool _24;
    /* 0x24 */ bool _25;
};
