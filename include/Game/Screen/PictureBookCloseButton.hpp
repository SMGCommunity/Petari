#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;

class PictureBookCloseButton : public LayoutActor {
public:
    /// @brief Creates a new `PictureBookCloseButton`.
    PictureBookCloseButton(bool);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void control();

    void disappear();
    bool trySelect();
    bool isSelected() const;
    void exeSelect();
    void exeSelected();
    void exeNotSelected();

private:
    /* 0x20 */ ButtonPaneController* mPaneCtrl;
    /* 0x24 */ bool mCanCloseWithPad;
};
