#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;

class PrologueLetter : LayoutActor {
public:
    PrologueLetter(const char* pName);

    virtual ~PrologueLetter();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();

    void pauseOff();
    void exeAppear();
    void exeWait();
    void exeDisappear();

private:
    /* 0x20 */ IconAButton* mAButtonIcon;
};
