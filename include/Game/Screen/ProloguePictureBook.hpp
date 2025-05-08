#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;

class ProloguePictureBook : LayoutActor {
public:
    ProloguePictureBook();

    virtual ~ProloguePictureBook();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();

    void exeActive();
    void exePlaying();
    void exeKeyWait();
    void exeEnd();
    bool isEnd() const;

private:
    /* 0x20 */ IconAButton* mAButtonIcon;
    /* 0x24 */ u32 mPage;
};
