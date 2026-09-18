#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CountUpPaneRumbler;
class CounterLayoutAppearer;

class PurpleCoinCounter : public LayoutActor {
public:
    PurpleCoinCounter(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void validate();
    void updateCounter();
    void exeAppear();
    void exeWait();

public:
    /* 0x20 */ s32 mPurpleCoinNum;
    /* 0x24 */ s32 mPurpleCoinDisplayNum;
    /* 0x28 */ s32 mInvalidCountUpFrame;
    /* 0x2C */ CounterLayoutAppearer* mLayoutAppearer;
    /* 0x30 */ CountUpPaneRumbler* mPaneRumbler;
    /* 0x34 */ bool mIsValid;
};
