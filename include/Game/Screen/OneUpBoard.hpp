#pragma once

#include "Game/Screen/LayoutActor.hpp"

class OneUpBoard : public LayoutActor {
public:
    OneUpBoard();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void control();

    void popup(s32 oneUpNum);
    void calcPos();
    void exeWait();
    void exeAppear();
};
