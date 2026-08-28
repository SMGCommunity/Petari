#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;

class ProloguePictureBook : public LayoutActor {
public:
    ProloguePictureBook();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();
    /* 0x2C */ virtual void control();

    void exeActive();
    void exePlaying();
    void exeKeyWait();
    void exeEnd();
    bool isEnd() const;

private:
    /* 0x20 */ IconAButton* mIconAButton;
    /* 0x24 */ u32 mPage;
};
