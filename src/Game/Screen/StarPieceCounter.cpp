#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Screen/CounterLayoutController.hpp"
#include "Game/Screen/StarPieceCounter.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const f32 cAppearOffsetCenterY = 30.0f;
    const f32 cAppearOffsetRightX = 50.0f;
    const s32 cInvalidCountUpInterval = 3;
    const s32 cHideFrameMin = 3;
    const s32 cDecCounterValueFast = 10;
    const f32 cBaseOffsetEatTicoY = -120.0f;
    const f32 cBaseOffsetTicoFatY = -94.0f;
    const s32 cHideFrameMinTicoEat = 20;
}; // namespace

namespace NrvStarPieceCounter {
    NEW_NERVE(StarPieceCounterNrvHide, StarPieceCounter, Hide);
    NEW_NERVE(StarPieceCounterNrvAppear, StarPieceCounter, Appear);
    NEW_NERVE(StarPieceCounterNrvWait, StarPieceCounter, Wait);
    NEW_NERVE(StarPieceCounterNrvDisappear, StarPieceCounter, Disappear);
}; // namespace NrvStarPieceCounter

StarPieceCounter::StarPieceCounter(const char* pName)
    : LayoutActor(pName, true),
      mStarPieceNum(0),
      mStarPieceDisplayNum(0),
      mInvalidCountUpFrame(0),
      mLayoutAppearer(nullptr),
      mPaneRumbler(nullptr),
      mFollowPos(0.0f, 0.0f),
      _3C(0),
      mMode(0) {
}

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

    initEffectKeeper(0, nullptr, nullptr);

    mStarPieceNum = MR::getStarPieceNum();
    mStarPieceDisplayNum = mStarPieceNum;

    initNerve(&NrvStarPieceCounter::StarPieceCounterNrvHide::sInstance);
    MR::connectToSceneLayout(this);
    appear();
}

void StarPieceCounter::appear() {
    mLayoutAppearer->reset();
    mPaneRumbler->reset();

    mInvalidCountUpFrame = 0;
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

void StarPieceCounter::forceSync() {
    mInvalidCountUpFrame = 0;
    mStarPieceNum = MR::getStarPieceNum();
    mStarPieceDisplayNum = mStarPieceNum;
}

bool StarPieceCounter::tryOnModeTicoEat(bool param1) {
    int mode = 1;

    if (param1) {
        mode = 2;
    }

    return tryChangeModeTicoEat(mode);
}

bool StarPieceCounter::tryOffModeTicoEat() {
    return tryChangeModeTicoEat(0);
}

void StarPieceCounter::control() {
    updateCounter();
    mLayoutAppearer->update();
    mPaneRumbler->update();
}

void StarPieceCounter::updateCounter() {
    const char* pPaneName;

    mStarPieceNum = MR::getStarPieceNum();

    updateCounterValue();

    if (isDispCenter()) {
        if (mStarPieceDisplayNum >= 1000) {
            pPaneName = "TPosition1000";
        } else if (mStarPieceDisplayNum >= 100) {
            pPaneName = "TPosition0100";
        } else if (mStarPieceDisplayNum >= 10) {
            pPaneName = "TPosition0010";
        } else {
            pPaneName = "TPosition0001";
        }

        MR::setTextBoxNumberRecursive(this, "TCounter", mStarPieceDisplayNum);
    } else {
        if (mStarPieceDisplayNum >= 100) {
            pPaneName = "Position100";
        } else if (mStarPieceDisplayNum >= 10) {
            pPaneName = "Position010";
        } else {
            pPaneName = "Position001";
        }

        MR::setTextBoxNumberRecursive(this, "Counter", mStarPieceDisplayNum);
    }

    MR::copyPaneTrans(&mFollowPos, this, pPaneName);
}

void StarPieceCounter::updateCounterValue() {
    if (isNerve(&NrvStarPieceCounter::StarPieceCounterNrvDisappear::sInstance)) {
        return;
    }

    if (mInvalidCountUpFrame > 0) {
        mInvalidCountUpFrame--;
    } else if (mStarPieceDisplayNum != mStarPieceNum) {
        if (isNerve(&NrvStarPieceCounter::StarPieceCounterNrvWait::sInstance)) {
            if (mStarPieceDisplayNum < mStarPieceNum) {
                mStarPieceDisplayNum++;

                MR::startAnim(this, "Flash", 0);
                MR::emitEffect(this, MR::isStageAstroLocation()
                                         ? "TStarPieceCounterLight"
                                         : "StarPieceCounterLight");
            } else if (mMode == 1) {
                if (mStarPieceDisplayNum - mStarPieceNum > 10) {
                    mStarPieceDisplayNum -= cDecCounterValueFast;
                } else {
                    mStarPieceDisplayNum = mStarPieceNum;
                }
            } else {
                mStarPieceDisplayNum--;
            }

            mInvalidCountUpFrame = cInvalidCountUpInterval;

            mPaneRumbler->start();
        }

        if (!isNerve(&NrvStarPieceCounter::StarPieceCounterNrvAppear::sInstance)) {
            if (!isNerve(&NrvStarPieceCounter::StarPieceCounterNrvWait::sInstance)) {
                setNerve(&NrvStarPieceCounter::StarPieceCounterNrvAppear::sInstance);
            } else {
                setNerve(&NrvStarPieceCounter::StarPieceCounterNrvWait::sInstance);
            }
        }
    }
}

bool StarPieceCounter::isValidAppearSituation() const {
    if (mMode != 0) {
        return !MR::isExistStarPointerGuidanceFrame1P();
    }

    return MR::isStarPointerInScreenAnyPort(0) && !CounterLayoutController::isInvalidSystemStateShowCounter();
}

bool StarPieceCounter::isDispCenter() const {
    return MR::isStageAstroLocation() || mMode != 0 || _3C == 2;
}

bool StarPieceCounter::tryChangeModeTicoEat(int mode) {
    if (isNerve(&NrvStarPieceCounter::StarPieceCounterNrvAppear::sInstance) || isNerve(&NrvStarPieceCounter::StarPieceCounterNrvDisappear::sInstance)) {
        return false;
    }

    if (isNerve(&NrvStarPieceCounter::StarPieceCounterNrvWait::sInstance)) {
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvDisappear::sInstance);

        return false;
    }

    if (isNerve(&NrvStarPieceCounter::StarPieceCounterNrvHide::sInstance)) {
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvHide::sInstance);
    }

    forceSync();

    mMode = mode;

    return true;
}

void StarPieceCounter::exeHide() {
    if (MR::isFirstStep(this)) {
        mInvalidCountUpFrame = 0;

        MR::hideLayout(this);
    }

    s32 step = mMode != 0
                   ? cHideFrameMinTicoEat
                   : cHideFrameMin;

    if (MR::isGreaterStep(this, step) && isValidAppearSituation()) {
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvAppear::sInstance);
    }
}

void StarPieceCounter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::startAnim(this, "Wait", 1);

        if (isDispCenter()) {
            MR::showPaneRecursive(this, "TSPieceCounter");
            MR::hidePaneRecursive(this, "StarPieceCounter");
            mLayoutAppearer->setAppearOffset(TVec2f(0.0f, cAppearOffsetCenterY));

            if (MR::isStageAstroLocation()) {
                MR::showPaneRecursive(this, "PicTStarPiece");
                MR::hidePaneRecursive(this, "PicNStarPiece");
            } else {
                MR::showPaneRecursive(this, "PicNStarPiece");
                MR::hidePaneRecursive(this, "PicTStarPiece");
            }
        } else {
            MR::showPaneRecursive(this, "StarPieceCounter");
            MR::hidePaneRecursive(this, "TSPieceCounter");
            mLayoutAppearer->setAppearOffset(TVec2f(cAppearOffsetRightX, 0.0f));
        }

        f32 y = 0.0f;

        if (mMode != 0) {
            y = cBaseOffsetTicoFatY;
        } else if (_3C == 2) {
            y = cBaseOffsetEatTicoY;
        }

        mLayoutAppearer->appear(TVec2f(0.0f, y));
    }

    if (mLayoutAppearer->isAppeared()) {
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvWait::sInstance);
    }
}

void StarPieceCounter::exeWait() {
    if (isValidAppearSituation()) {
        return;
    }

    if (mMode != 0 || (_3C == 0 && mStarPieceDisplayNum == mStarPieceNum && CounterLayoutController::isWaitToDisappearCounter(this))) {
        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvDisappear::sInstance);
    }
}

void StarPieceCounter::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mLayoutAppearer->disappear();
    }

    if (mLayoutAppearer->isDisappeared()) {
        if (_3C == 2) {
            _3C = 0;
        }

        setNerve(&NrvStarPieceCounter::StarPieceCounterNrvHide::sInstance);
    }
}
