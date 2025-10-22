#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Screen/PurpleCoinCounter.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

namespace {
    const s32 cInvalidCountUpInterval = 3;
};

namespace NrvPurpleCoinCounter {
    NEW_NERVE(PurpleCoinCounterNrvAppear, PurpleCoinCounter, Appear);
    NEW_NERVE(PurpleCoinCounterNrvWait, PurpleCoinCounter, Wait);
};

PurpleCoinCounter::PurpleCoinCounter(const char* pName) :
    LayoutActor(pName, true),
    mPurpleCoinNum(0),
    mPurpleCoinDisplayNum(0),
    mInvalidCountUpFrame(0),
    mLayoutAppearer(nullptr),
    mPaneRumbler(nullptr),
    mIsValid(false)
{
    
}

void PurpleCoinCounter::init(const JMapInfoIter& rIter) {
    initLayoutManager("PurpleCoinCounter", 2);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    initEffectKeeper(0, nullptr, nullptr);

    mLayoutAppearer = new CounterLayoutAppearer(this, TVec2f(-50.0f, 0.0f));
    mPaneRumbler = new CountUpPaneRumbler(this, "Counter");

    initNerve(&NrvPurpleCoinCounter::PurpleCoinCounterNrvAppear::sInstance);
    MR::connectToSceneLayout(this);
}

void PurpleCoinCounter::appear() {
    if (!mIsValid) {
        return;
    }

    if (!MR::isDead(this)) {
        return;
    }

    mLayoutAppearer->reset();
    mPaneRumbler->reset();

    mInvalidCountUpFrame = 0;

    MR::setTextBoxFormatRecursive(this, "Counter", L"%03d", mPurpleCoinDisplayNum);
    setNerve(&NrvPurpleCoinCounter::PurpleCoinCounterNrvAppear::sInstance);
    LayoutActor::appear();
    MR::requestMovementOn(this);
}

void PurpleCoinCounter::validate() {
    mIsValid = true;

    appear();
}

void PurpleCoinCounter::control() {
    updateCounter();
    mLayoutAppearer->update();
    mPaneRumbler->update();
}

void PurpleCoinCounter::updateCounter() {
    mPurpleCoinNum = MR::getPurpleCoinNum();

    if (mInvalidCountUpFrame > 0) {
        mInvalidCountUpFrame--;
    }
    else if (mPurpleCoinDisplayNum < mPurpleCoinNum) {
        if (isNerve(&NrvPurpleCoinCounter::PurpleCoinCounterNrvWait::sInstance)) {
            mInvalidCountUpFrame = cInvalidCountUpInterval;
            mPurpleCoinDisplayNum++;

            MR::startAnim(this, "Flash", 0);
            MR::emitEffect(this, "PurpleCoinCounterLight");
            mPaneRumbler->start();
        }

        if (!isNerve(&NrvPurpleCoinCounter::PurpleCoinCounterNrvAppear::sInstance)) {
            if (!isNerve(&NrvPurpleCoinCounter::PurpleCoinCounterNrvWait::sInstance)) {
                setNerve(&NrvPurpleCoinCounter::PurpleCoinCounterNrvAppear::sInstance);
            }
            else {
                setNerve(&NrvPurpleCoinCounter::PurpleCoinCounterNrvWait::sInstance);
            }
        }
    }

    MR::setTextBoxFormatRecursive(this, "Counter", L"%03d", mPurpleCoinDisplayNum);
}

void PurpleCoinCounter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        mLayoutAppearer->appear(TVec2f(0.0f, 0.0f));
        MR::startAnim(this, "Wait", 1);
    }

    if (mLayoutAppearer->isAppeared()) {
        setNerve(&NrvPurpleCoinCounter::PurpleCoinCounterNrvWait::sInstance);
    }
}

void PurpleCoinCounter::exeWait() {

}
