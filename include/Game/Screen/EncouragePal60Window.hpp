#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;

class EncouragePal60Window : LayoutActor {
public:
    EncouragePal60Window();

    virtual ~EncouragePal60Window();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    void exeAppear();
    void exeDisplay();
    void exeDisappear();

private:
    /* 0x20 */ IconAButton* mAButtonIcon;
};
