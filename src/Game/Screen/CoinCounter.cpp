#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CoinCounter.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Screen/CounterLayoutController.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvCoinCounter {
    NEW_NERVE(CoinCounterNrvHide, CoinCounter, Hide);
    NEW_NERVE(CoinCounterNrvAppear, CoinCounter, Appear);
    NEW_NERVE(CoinCounterNrvWait, CoinCounter, Wait);
    NEW_NERVE(CoinCounterNrvDisappear, CoinCounter, Disappear);
};

CoinCounter::CoinCounter(const char* pName) :
    LayoutActor(pName, true),
    mCoinNum(0),
    mCoinDisplayNum(0),
    mDisplayUpdateFrame(0),
    mLayoutAppearer(NULL),
    mPaneRumbler(NULL),
    mIsForceAppear(false),
    mFollowPos(0.0f, 0.0f)
{}

void CoinCounter::init(const JMapInfoIter& rIter) {
    initLayoutManager("CoinCounter", 2);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    MR::createAndAddPaneCtrl(this, "CoinCounter", 1);
    MR::setFollowPos(&mFollowPos, this, "CoinCounter");
    initEffectKeeper(0, NULL, NULL);

    mLayoutAppearer = new CounterLayoutAppearer(this, TVec2f(50.0f, 0.0f));
    mPaneRumbler = new CountUpPaneRumbler(this, "Counter");

    initNerve(&NrvCoinCounter::CoinCounterNrvHide::sInstance);
    MR::connectToSceneLayout(this);
    appear();
}

void CoinCounter::appear() {
    mLayoutAppearer->reset();
    mPaneRumbler->reset();

    mDisplayUpdateFrame = 0;
    mIsForceAppear = false;

    MR::hideLayout(this);
    setNerve(&NrvCoinCounter::CoinCounterNrvHide::sInstance);
    LayoutActor::appear();
}

void CoinCounter::forceAppear() {
    if (!isNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance)) {
        appear();
        setNerve(&NrvCoinCounter::CoinCounterNrvAppear::sInstance);
    }

    mIsForceAppear = true;
}

void CoinCounter::disappear() {
    mIsForceAppear = false;

    setNerve(&NrvCoinCounter::CoinCounterNrvDisappear::sInstance);
}

bool CoinCounter::isWait() const {
    return isNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance);
}

void CoinCounter::control() {
    updateCounter();
    mLayoutAppearer->update();
    mPaneRumbler->update();
}

void CoinCounter::updateCounter() {
    mCoinNum = MR::getCoinNum();

    if (mDisplayUpdateFrame > 0) {
        mDisplayUpdateFrame--;
    }
    else if (mCoinDisplayNum < mCoinNum) {
        if (isNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance)) {
            mDisplayUpdateFrame = 3;
            mCoinDisplayNum++;

            MR::startAnim(this, "Flash", 0);
            MR::emitEffect(this, "CoinCounterLight");
            mPaneRumbler->start();
        }

        if (!isNerve(&NrvCoinCounter::CoinCounterNrvAppear::sInstance)) {
            if (!isNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance)) {
                setNerve(&NrvCoinCounter::CoinCounterNrvAppear::sInstance);
            }
            else {
                setNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance);
            }
        }
    }

    char* pPaneName = "Position001";

    if (mCoinDisplayNum >= 100) {
        pPaneName = "Position100";
    }
    else if (mCoinDisplayNum >= 10) {
        pPaneName = "Position010";
    }

    MR::copyPaneTrans(&mFollowPos, this, pPaneName);
    MR::setTextBoxNumberRecursive(this, "Counter", mCoinDisplayNum);
}

void CoinCounter::exeHide() {
    if (MR::isFirstStep(this)) {
        mDisplayUpdateFrame = 0;

        MR::hideLayout(this);
    }
}

void CoinCounter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);

        if (MR::isStageAstroLocation()) {
            mLayoutAppearer->appear(TVec2f(0.0f, 40.0f));
        }
        else {
            mLayoutAppearer->appear(TVec2f(0.0f, 0.0f));
        }

        MR::startAnim(this, "Wait", 1);
    }

    if (mLayoutAppearer->isAppeared()) {
        setNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance);
    }
}

void CoinCounter::exeWait() {
    if (mIsForceAppear) {
        return;
    }

    if (mCoinDisplayNum != mCoinNum) {
        return;
    }

    if (CounterLayoutController::isWaitToDisappearCounter(this)) {
        setNerve(&NrvCoinCounter::CoinCounterNrvDisappear::sInstance);
    }
}

void CoinCounter::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mLayoutAppearer->disappear();
    }

    if (mLayoutAppearer->isDisappeared()) {
        setNerve(&NrvCoinCounter::CoinCounterNrvHide::sInstance);
    }
}

CoinCounter::~CoinCounter() {
    
}
