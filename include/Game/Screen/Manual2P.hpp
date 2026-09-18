#pragma once

#include "Game/Screen/LayoutActor.hpp"

class BackButton;
class ButtonPaneController;

class Manual2P : public LayoutActor {
public:
    /// @brief Creates a new `Manual2P`.
    /// @param pName The pointer to the null-terminated name of the object.
    Manual2P(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    bool isClosed() const;
    void exeAppear();
    void exeWait();
    void exeScrollRight();
    void exeScrollRightAfter();
    void exeScrollLeft();
    void exeScrollLeftAfter();
    void exeDisappear();
    void reflectPageIndex();
    bool checkSelectedBackButton();

public:
    /* 0x20 */ s32 mPageIndex;
    /* 0x24 */ s32 _24;
    /* 0x28 */ ButtonPaneController* mLeftPaneCtrl;
    /* 0x2C */ ButtonPaneController* mRightPaneCtrl;
    /* 0x30 */ bool _30;
    /* 0x31 */ bool _31;
    /* 0x34 */ BackButton* mBackButton;
};
