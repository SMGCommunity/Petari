#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CountUpPaneRumbler;
class CounterLayoutAppearer;

class PlayerLeft : public LayoutActor {
public:
    /// @brief Creates a new `PlayerLeft`.
    /// @param pName The pointer to the null-terminated name of the object.
    PlayerLeft(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void forceAppear();
    void disappear();
    bool isWait() const;
    void updateCounter();
    bool isValidAppearSituation() const;
    void exeHide();
    void exeAppear();
    void exeWait();
    void exeDisappear();

public:
    /* 0x20 */ s32 mPlayerLeft;
    /* 0x24 */ CounterLayoutAppearer* mLayoutAppearer;
    /* 0x28 */ CountUpPaneRumbler* mPaneRumbler;
    /* 0x2C */ bool mIsForceAppear;
};
