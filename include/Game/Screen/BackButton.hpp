#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;

class BackButton : public LayoutActor {
public:
    BackButton(const char* pName, bool param2);

    virtual ~BackButton();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void disappear();
    bool isHidden() const;
    bool isAppearing() const;
    bool isDisappearing() const;
    bool isPointing() const;

    /* 0x20 */ ButtonPaneController* mPaneCtrl;
    /* 0x24 */ bool _24;
    /* 0x24 */ bool _25;
};
