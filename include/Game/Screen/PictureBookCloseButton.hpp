#pragma once

#include "Game/Screen/LayoutActor.hpp"

class PictureBookCloseButton : public LayoutActor {
public:
    /// @brief Creates a new `PictureBookCloseButton`.
    PictureBookCloseButton(bool);

    /// @brief Destroys the `PictureBookCloseButton`.
    virtual ~PictureBookCloseButton();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

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
