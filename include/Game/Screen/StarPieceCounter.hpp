#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CountUpPaneRumbler;
class CounterLayoutAppearer;

class StarPieceCounter : public LayoutActor {
public:
    /// @brief Creates a new `StarPieceCounter`.
    /// @param pName The pointer to the null-terminated name of the object.
    StarPieceCounter(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void forceAppear(bool);
    void disappear(bool);
    bool isWait() const;
    void forceSync();
    bool tryOnModeTicoEat(bool);
    bool tryOffModeTicoEat();
    void updateCounter();
    void updateCounterValue();
    bool isValidAppearSituation() const;
    bool isDispCenter() const;
    bool tryChangeModeTicoEat(int);
    void exeHide();
    void exeAppear();
    void exeWait();
    void exeDisappear();

    /* 0x20 */ s32 mStarPieceNum;
    /* 0x24 */ s32 mStarPieceDisplayNum;
    /* 0x28 */ s32 mInvalidCountUpFrame;
    /* 0x2C */ CounterLayoutAppearer* mLayoutAppearer;
    /* 0x30 */ CountUpPaneRumbler* mPaneRumbler;
    /* 0x34 */ TVec2f mFollowPos;
    /* 0x38 */ s32 _3C;
    /* 0x3C */ int mMode;
};
