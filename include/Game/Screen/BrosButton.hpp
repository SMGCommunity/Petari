#pragma once

#include "Game/LiveActor/Nerve.hpp"
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

namespace {
    NERVE_DECL(BrosButtonNrvAppear, BrosButton, BrosButton::exeAppear);
    NERVE_DECL(BrosButtonNrvSelect, BrosButton, BrosButton::exeSelect);
    NERVE_DECL(BrosButtonNrvDecide, BrosButton, BrosButton::exeDecide);
    NERVE_DECL(BrosButtonNrvDisappear, BrosButton, BrosButton::exeDisappear);
};
