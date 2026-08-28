#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;

class BrosButton : public LayoutActor {
public:
    BrosButton(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void control();

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
