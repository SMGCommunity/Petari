#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/BackButton.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/Manual2P.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>
#include <cstdio>

namespace {
    NEW_NERVE(Manual2PNrvAppear, Manual2P, Appear);
    NEW_NERVE(Manual2PNrvWait, Manual2P, Wait);
    NEW_NERVE(Manual2PNrvScrollRight, Manual2P, ScrollRight);
    NEW_NERVE(Manual2PNrvScrollRightAfter, Manual2P, ScrollRightAfter);
    NEW_NERVE(Manual2PNrvScrollLeft, Manual2P, ScrollLeft);
    NEW_NERVE(Manual2PNrvScrollLeftAfter, Manual2P, ScrollLeftAfter);
    NEW_NERVE(Manual2PNrvDisappear, Manual2P, Disappear);
};

Manual2P::Manual2P(const char* pName) :
    LayoutActor(pName, true),
    mPageIndex(0),
    _24(0),
    mLeftPaneCtrl(NULL),
    mRightPaneCtrl(NULL),
    _30(false),
    _31(false),
    mBackButton(NULL)
{
    
}

void Manual2P::init(const JMapInfoIter& rIter) {
    s32 i;
    char messageId[128];

    initLayoutManager("P2Manual", 2);
    MR::invalidateParentAnim(this);
    MR::createAndAddPaneCtrl(this, "LeftButton", 2);

    mLeftPaneCtrl = new ButtonPaneController(this, "LeftButton", "PicLButton", 0, true);
    mLeftPaneCtrl->_22 = 0;

    MR::createAndAddPaneCtrl(this, "RightButton", 2);

    mRightPaneCtrl = new ButtonPaneController(this, "RightButton", "PicRButton", 0, true);
    mRightPaneCtrl->_22 = 0;

    mBackButton = new BackButton("マニュアルの戻るボタン", true);
    mBackButton->initWithoutIter();

    initNerve(&Manual2PNrvAppear::sInstance);
    MR::connectToSceneLayout(this);

    do {
        snprintf(messageId, sizeof(messageId), "%s%03d", "2PGuidance", i + 1);
        i++;
    }
    while (MR::isExistGameMessage(messageId));

    _24 = i;
}

void Manual2P::appear() {
    LayoutActor::appear();
    setNerve(&Manual2PNrvAppear::sInstance);

    mPageIndex = 0;

    MR::startAnim(this, "Picture", 1);

    if (_24 > MR::getAnimCtrl(this, 1)->mEndFrame) {
        _24 = MR::getAnimCtrl(this, 1)->mEndFrame;
    }
}

bool Manual2P::isClosed() const {
    return MR::isDead(this);
}

void Manual2P::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        mLeftPaneCtrl->appear();
        mRightPaneCtrl->appear();
        mBackButton->appear();
        reflectPageIndex();
    }

    if (MR::isAnimStopped(this, 0)
        && mLeftPaneCtrl->isWait()
        && mRightPaneCtrl->isWait())
    {
        setNerve(&Manual2PNrvWait::sInstance);
    }
}

void Manual2P::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
    }

    if (_30) {
        mLeftPaneCtrl->trySelect();

        if (mLeftPaneCtrl->mIsSelected) {
            MR::startSystemSE("SE_SY_FILE_SEL_TIPS_PAGE", -1, -1);
            setNerve(&Manual2PNrvScrollLeft::sInstance);
            return;
        }

        if (MR::testSubPadStickTriggerLeft(0) || MR::testCorePadTriggerLeft(0)) {
            mLeftPaneCtrl->_24 = false;

            MR::startSystemSE("SE_SY_FILE_SEL_TIPS_PAGE", -1, -1);
            setNerve(&Manual2PNrvScrollLeft::sInstance);
            return;
        }
    }

    if (_31) {
        mRightPaneCtrl->trySelect();

        if (mRightPaneCtrl->mIsSelected) {
            MR::startSystemSE("SE_SY_FILE_SEL_TIPS_PAGE", -1, -1);
            setNerve(&Manual2PNrvScrollRight::sInstance);
            return;
        }

        if (MR::testSubPadStickTriggerRight(0) || MR::testCorePadTriggerRight(0)) {
            mRightPaneCtrl->_24 = false;

            MR::startSystemSE("SE_SY_FILE_SEL_TIPS_PAGE", -1, -1);
            setNerve(&Manual2PNrvScrollRight::sInstance);
            return;
        }
    }

    if (checkSelectedBackButton()) {
        setNerve(&Manual2PNrvDisappear::sInstance);
    }
}

void Manual2P::exeScrollRight() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "PageOut", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&Manual2PNrvScrollRightAfter::sInstance);
    }
}

void Manual2P::exeScrollRightAfter() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "PageOut", 0);

        mPageIndex++;

        reflectPageIndex();
    }

    if (MR::isAnimStopped(this, 0)) {
        mRightPaneCtrl->_24 = true;
        mRightPaneCtrl->forceToWait();
        setNerve(&Manual2PNrvWait::sInstance);
    }
}

void Manual2P::exeScrollLeft() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "PageIn", 0);
        MR::setAnimFrame(this, MR::getAnimCtrl(this, 0)->mEndFrame - 1.0f, 0);
        MR::getAnimCtrl(this, 0)->mSpeed = -1.0f;
    }

    if (MR::getAnimCtrl(this, 0)->mCurrentFrame + MR::getAnimCtrl(this, 0)->mSpeed <= 0.0f) {
        setNerve(&Manual2PNrvScrollLeftAfter::sInstance);
    }
}

void Manual2P::exeScrollLeftAfter() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "PageIn", 0);
        MR::setAnimFrame(this, MR::getAnimCtrl(this, 0)->mEndFrame - 1.0f, 0);
        MR::getAnimCtrl(this, 0)->mSpeed = -1.0f;

        mPageIndex--;

        reflectPageIndex();
    }

    if (MR::getAnimCtrl(this, 0)->mCurrentFrame - MR::getAnimCtrl(this, 0)->mSpeed <= 0.0f) {
        mLeftPaneCtrl->_24 = true;
        mLeftPaneCtrl->forceToWait();
        setNerve(&Manual2PNrvScrollLeftAfter::sInstance);
    }
}

void Manual2P::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
        reflectPageIndex();
    }

    if (MR::isAnimStopped(this, 0) && mBackButton->isHidden()) {
        kill();
    }
}

void Manual2P::control() {
    mLeftPaneCtrl->update();
    mRightPaneCtrl->update();

    if (mLeftPaneCtrl->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON", -1, -1);
    }

    if (mRightPaneCtrl->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON", -1, -1);
    }
}

// Manual2P::reflectPageIndex

bool Manual2P::checkSelectedBackButton() {
    if (mBackButton->_24) {
        return true;
    }

    if (MR::testSystemTriggerB()) {
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL", -1, -1);
        mBackButton->disappear();

        return true;
    }

    return false;
}
