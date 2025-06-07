#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;

class BrosButton : LayoutActor {
public:
    BrosButton(const char* pName);

    virtual ~BrosButton();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void appear(bool);
    void disappear();
    bool isSelected() const;
    bool isSelectedMario() const;
    void resume();
    void exeAppear();
    void exeSelect();
    void exeDecide();
    void exeDisappear();

private:
    /* 0x20 */ bool mIsSelectedMario;
    /* 0x24 */ ButtonPaneController* mPaneCtrl;
};
