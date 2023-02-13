#include "Game/Screen/CoinCounter.h"
#include "Game/Screen/CounterLayoutController.h"
#include "Game/Util.h"

CoinCounter::CoinCounter(const char *pName) : LayoutActor(pName, true) {
    mCoinCount = 0;
    _24 = 0;
    _28 = 0;
    mLayoutAppearer = nullptr;
    mPaneRumbler = nullptr;
    mIsAppear = false;
    mFollowPos.x = 0.0f;
    mFollowPos.y = 0.0f;
}

void CoinCounter::init(const JMapInfoIter &rIter) {
    initLayoutManager("CoinCounter", 2);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    MR::createAndAddPaneCtrl(this, "CoinCounter", 1);
    MR::setFollowPos(&mFollowPos, this, "CoinCounter");
    initEffectKeeper(0, nullptr, nullptr);
    mLayoutAppearer = new CounterLayoutAppearer(this, TVec2f(50.0f, 0.0f));
    mPaneRumbler = new CountUpPaneRumbler(this, "Counter");
    initNerve(&NrvCoinCounter::CoinCounterNrvHide::sInstance);
    MR::connectToSceneLayout(this);
    appear();
}

void CoinCounter::appear() {
    mLayoutAppearer->reset();
    mPaneRumbler->reset();
    _28 = 0;
    mIsAppear = false;
    MR::hideLayout(this);
    setNerve(&NrvCoinCounter::CoinCounterNrvHide::sInstance);
    LayoutActor::appear();
}

void CoinCounter::forceAppear() {
    if (!isNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance)) {
        appear();
        setNerve(&NrvCoinCounter::CoinCounterNrvAppear::sInstance);
    }
    
    mIsAppear = true;
}

void CoinCounter::disappear() {
    mIsAppear = false;
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
    s32 coinNum = MR::getCoinNum();
    s32 var = _28;
    mCoinCount = coinNum;

    if (var > 0) {
        _28 = var - 1;
    }
    else {
        if (_24 < coinNum) {
            if (isNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance)) {
                u32 v4 = _24;
                _28 = 3;
                _24 = v4 + 1;
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
    }

    char* pane = "Position001";
    if (_24 >= 100) {
        pane = "Position100";
    }
    else {
        if (_24 >= 10) {
            pane = "Position010";
        }
    }

    MR::copyPaneTrans(&mFollowPos, this, pane);
    MR::setTextBoxNumberRecursive(this, "Counter", _24);
}

void CoinCounter::exeHide() {
    if (MR::isFirstStep(this)) {
        _28 = 0;
        MR::hideLayout(this);
    }
}

void CoinCounter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        if (MR::isStageAstroLocation()) {
            TVec2f vec;
            vec.x = 0.0f;
            vec.y = 40.0f;
            mLayoutAppearer->appear(vec);
        }
        else {
            TVec2f vec;
            vec.x = 0.0f;
            vec.y = 0.0f;
            mLayoutAppearer->appear(vec);
        }

        MR::startAnim(this, "Wait", 1);
    }

    if (mLayoutAppearer->isAppeared()) {
        setNerve(&NrvCoinCounter::CoinCounterNrvWait::sInstance);
    }
}

void CoinCounter::exeWait() {
    if (!mIsAppear && _24 == mCoinCount) {
        if (CounterLayoutController::isWaitToDisappearCounter(this)) {
            setNerve(&NrvCoinCounter::CoinCounterNrvDisappear::sInstance);
        }
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

namespace NrvCoinCounter {
    INIT_NERVE(CoinCounterNrvHide);
    INIT_NERVE(CoinCounterNrvAppear);
    INIT_NERVE(CoinCounterNrvWait);
    INIT_NERVE(CoinCounterNrvDisappear);

    void CoinCounterNrvDisappear::execute(Spine *pSpine) const {
        CoinCounter* counter = reinterpret_cast<CoinCounter*>(pSpine->mExecutor);
        counter->exeDisappear(); 
    }

    void CoinCounterNrvWait::execute(Spine *pSpine) const {
        CoinCounter* counter = reinterpret_cast<CoinCounter*>(pSpine->mExecutor);
        counter->exeWait();
    }

    void CoinCounterNrvAppear::execute(Spine *pSpine) const {
        CoinCounter* counter = reinterpret_cast<CoinCounter*>(pSpine->mExecutor);
        counter->exeAppear();
    }

    void CoinCounterNrvHide::execute(Spine *pSpine) const {
        CoinCounter* counter = reinterpret_cast<CoinCounter*>(pSpine->mExecutor);
        counter->exeHide();
    }
};