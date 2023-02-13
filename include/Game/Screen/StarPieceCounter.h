#pragma once

#include "Game/Screen/LayoutActor.h"
#include "Game/Screen/CounterLayoutAppearer.h"
#include "Game/Screen/CountUpPaneRumbler.h"

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

    s32 mStarPieceNum;                      // _20
    s32 _24;
    u32 _28;
    CounterLayoutAppearer* mAppearer;       // _2C
    CountUpPaneRumbler* mPaneRumbler;       // _30
    TVec2f mFollowPos;                      // _34
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