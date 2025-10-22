#pragma once

#include "Game/Screen/LayoutActor.hpp"

class OneUpBoard : public LayoutActor {
public:
    OneUpBoard();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void popup(s32 oneUpNum);
    void calcPos();
    void exeWait();
    void exeAppear();
};
