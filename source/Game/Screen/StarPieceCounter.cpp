#include "Game/Screen/StarPieceCounter.hpp"

StarPieceCounter::StarPieceCounter(const char *pName) : LayoutActor(pName, true) {
    mStarPieceNum = 0;
    _24 = 0;
    _28 = 0;
    mAppearer = nullptr;
    mPaneRumbler = nullptr;
    mFollowPos.x = 0.0f;
    mFollowPos.y = 0.0f;
    _3C = 0;
    _40 = 0;
}

void StarPieceCounter::init(const JMapInfoIter &rIter) {
    initLayoutManager("StarPieceCounter", 3);
    MR::createAndAddPaneCtrl(this, "TSPieceCounter", 1);
    MR::setFollowPos(&mFollowPos, this, "TSPieceCounter");
    mPaneRumbler = new CountUpPaneRumbler(this, "TCounter");
    mAppearer = new CounterLayoutAppearer(this, TVec2f(0.0f, 30.0f));

    if (!MR::isStageAstroLocation()) {
        MR::createAndAddPaneCtrl(this, "StarPieceCounter", 1);
        MR::setFollowPos(&mFollowPos, this, "StarPieceCounter");
        mPaneRumbler->connectPane(this, "Counter");
    }

    initEffectKeeper(0, nullptr, nullptr);
    s32 pieceNum = MR::getStarPieceNum();
    mStarPieceNum = pieceNum;
    _24 = pieceNum;
    initNerve(&NrvStarPieceCounter::StarPieceCounterNrvHide::sInstance);
    MR::connectToSceneLayout(this);
    appear();
}

void StarPieceCounter::appear() {
    mAppearer->reset();
    mPaneRumbler->reset();
    _28 = 0;
    _3C = 0;
    MR::hideLayout(this);
    forceSync();
    setNerve(&NrvStarPieceCounter::StarPieceCounterNrvHide::sInstance);
    LayoutActor::appear();
    MR::requestMovementOn(this);
}

void StarPieceCounter::forceAppear(bool a1) {
    if (a1 || !StarPieceUtil::isDeadOrWaiting(this)) {
        appear();
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvAppear::sInstance);
    }

    s32 v5 = 1;
    if (a1) {
        v5 = 2;
    }

    _3C = v5;
}

void StarPieceCounter::disappear(bool a1) {
    if (a1 || !isValidAppearSituation()) {
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvDisappear::sInstance);
    }

    if (!a1) {
        _3C = 0;
    }
}

bool StarPieceCounter::isWait() const {
    bool isWait = false;

    if (!MR::isDead(this) && isNerve(&NrvStarPieceCounter::StarPieceCounterNrvWait::sInstance)) {
        isWait = true;
    }

    return isWait;
}