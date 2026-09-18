#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CountUpPaneRumbler;
class CounterLayoutAppearer;

class CoinCounter : public LayoutActor {
public:
    /// @brief Creates a new `CoinCounter`.
    /// @param pName The pointer to the null-terminated name of the object.
    CoinCounter(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void forceAppear();
    void disappear();
    bool isWait() const;
    void updateCounter();
    void exeHide();
    void exeAppear();
    void exeWait();
    void exeDisappear();

public:
    /* 0x20 */ s32 mCoinNum;
    /* 0x24 */ s32 mCoinDisplayNum;
    /* 0x28 */ s32 mInvalidCountUpFrame;
    /* 0x2C */ CounterLayoutAppearer* mLayoutAppearer;
    /* 0x30 */ CountUpPaneRumbler* mPaneRumbler;
    /* 0x34 */ bool mIsForceAppear;
    /* 0x38 */ TVec2f mFollowPos;
};
