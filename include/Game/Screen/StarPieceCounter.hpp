#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"

class StarPieceCounter : public LayoutActor {
public:
    StarPieceCounter(const char *);

    virtual ~StarPieceCounter();
    virtual void init(const JMapInfoIter &);
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

    s32 mStarPieceNum;                      // 0x20
    s32 _24;
    u32 _28;
    CounterLayoutAppearer* mAppearer;       // 0x2C
    CountUpPaneRumbler* mPaneRumbler;       // 0x30
    TVec2f mFollowPos;                      // 0x34
    s32 _3C;
    int _40;
};

namespace NrvStarPieceCounter {
    NERVE_DECL(StarPieceCounterNrvHide, StarPieceCounter, StarPieceCounter::exeHide);
    NERVE_DECL(StarPieceCounterNrvAppear, StarPieceCounter, StarPieceCounter::exeAppear);
    NERVE_DECL(StarPieceCounterNrvWait, StarPieceCounter, StarPieceCounter::exeWait);
    NERVE_DECL(StarPieceCounterNrvDisappear, StarPieceCounter, StarPieceCounter::exeDisappear);
};

namespace StarPieceUtil {
    inline bool isDeadOrWaiting(StarPieceCounter *pCounter) {
        bool isDeadAndWaiting = false;

        if (!MR::isDead(pCounter) && pCounter->isNerve(&NrvStarPieceCounter::StarPieceCounterNrvWait::sInstance)) {
            isDeadAndWaiting = true;
        }

        return isDeadAndWaiting;
    }
};