#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Screen/StarPieceCounter.hpp"

namespace NrvStarPieceCounter {
    NEW_NERVE(StarPieceCounterNrvHide, StarPieceCounter, Hide);
    NEW_NERVE(StarPieceCounterNrvAppear, StarPieceCounter, Appear);
    NEW_NERVE(StarPieceCounterNrvWait, StarPieceCounter, Wait);
    NEW_NERVE(StarPieceCounterNrvDisappear, StarPieceCounter, Disappear);
};

StarPieceCounter::StarPieceCounter(const char* pName) :
    LayoutActor(pName, true),
    mStarPieceNum(0),
    mStarPieceDisplayNum(0),
    mDisplayUpdateFrame(0),
    mLayoutAppearer(NULL),
    mPaneRumbler(NULL),
    mFollowPos(0.0f, 0.0f),
    _3C(0),
    mMode(0)
{}

void StarPieceCounter::init(const JMapInfoIter& rIter) {
    initLayoutManager("StarPieceCounter", 3);
    MR::createAndAddPaneCtrl(this, "TSPieceCounter", 1);
    MR::setFollowPos(&mFollowPos, this, "TSPieceCounter");

    mPaneRumbler = new CountUpPaneRumbler(this, "TCounter");
    mLayoutAppearer = new CounterLayoutAppearer(this, TVec2f(0.0f, 30.0f));

    if (!MR::isStageAstroLocation()) {
        MR::createAndAddPaneCtrl(this, "StarPieceCounter", 1);
        MR::setFollowPos(&mFollowPos, this, "StarPieceCounter");
        mPaneRumbler->connectPane(this, "Counter");
    }

    initEffectKeeper(0, NULL, NULL);

    mStarPieceNum = MR::getStarPieceNum();
    mStarPieceDisplayNum = mStarPieceNum;

    initNerve(&NrvStarPieceCounter::StarPieceCounterNrvHide::sInstance);
    MR::connectToSceneLayout(this);
    appear();
}

void StarPieceCounter::appear() {
    mLayoutAppearer->reset();
    mPaneRumbler->reset();

    mDisplayUpdateFrame = 0;
    _3C = 0;

    MR::hideLayout(this);
    forceSync();
    setNerve(&NrvStarPieceCounter::StarPieceCounterNrvHide::sInstance);
    LayoutActor::appear();
    MR::requestMovementOn(this);
}

void StarPieceCounter::forceAppear(bool param1) {
    if (param1 || !isWait()) {
        appear();
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvAppear::sInstance);
    }

    s32 v5 = 1;

    if (param1) {
        v5 = 2;
    }

    _3C = v5;
}

void StarPieceCounter::disappear(bool param1) {
    if (param1 || !isValidAppearSituation()) {
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvDisappear::sInstance);
    }

    if (!param1) {
        _3C = 0;
    }
}

bool StarPieceCounter::isWait() const {
    return !MR::isDead(this) && isNerve(&NrvStarPieceCounter::StarPieceCounterNrvWait::sInstance);
}
