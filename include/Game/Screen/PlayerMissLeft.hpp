#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CountUpPaneRumbler;
class CounterLayoutAppearer;

class PlayerMissLeft : public LayoutActor {
public:
    PlayerMissLeft(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void exeAppearWait();
    void exeAppear();
    void exeWait();
    void exeEnd();

public:
    /* 0x20 */ CounterLayoutAppearer* mLayoutAppearer;
    /* 0x24 */ CountUpPaneRumbler* mPaneRumbler;
};
