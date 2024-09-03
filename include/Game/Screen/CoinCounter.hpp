#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"

class CoinCounter : public LayoutActor {
public:
    CoinCounter(const char *);

    virtual ~CoinCounter();
    virtual void init(const JMapInfoIter &);
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

    s32 mCoinCount;                             // 0x20
    s32 _24;
    s32 _28;
    CounterLayoutAppearer* mLayoutAppearer;     // 0x2C
    CountUpPaneRumbler* mPaneRumbler;           // 0x30
    bool mIsAppear;                             // 0x34
    TVec2f mFollowPos;                          // 0x38
};

namespace NrvCoinCounter {
    NERVE(CoinCounterNrvHide);
    NERVE(CoinCounterNrvAppear);
    NERVE(CoinCounterNrvWait);
    NERVE(CoinCounterNrvDisappear);
};