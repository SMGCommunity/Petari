#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CountUpPaneRumbler;
class CounterLayoutAppearer;

class PlayerMissLeft : public LayoutActor {
public:
    PlayerMissLeft(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void control();

    void exeAppearWait();
    void exeAppear();
    void exeWait();
    void exeEnd();

private:
    /* 0x20 */ CounterLayoutAppearer* mLayoutAppearer;
    /* 0x24 */ CountUpPaneRumbler* mPaneRumbler;
};
