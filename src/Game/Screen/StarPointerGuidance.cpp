#include "Game/Screen/StarPointerGuidance.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    static const s32 hOutScreenTime = 60;
    static const s32 hRequestTime = 30;
    static const s32 hNotRequestEndTime = 30;
    static const s32 hEndTimeOutHide = 240;
};  // namespace

namespace NrvStarPointerGuidance {
    NEW_NERVE(HostTypeNrvAppear1P, StarPointerGuidance, Appear1P);
    NEW_NERVE(HostTypeNrvWait1P, StarPointerGuidance, Wait1P);
    NEW_NERVE(HostTypeNrvEnd1P, StarPointerGuidance, End1P);
    NEW_NERVE(HostTypeNrvAppearBlueStarGuide, StarPointerGuidance, AppearBlueStarGuide);
    NEW_NERVE(HostTypeNrvWaitBlueStarGuide, StarPointerGuidance, WaitBlueStarGuide);
    NEW_NERVE(HostTypeNrvEndBlueStarGuide, StarPointerGuidance, EndBlueStarGuide);
    NEW_NERVE(HostTypeNrvAppear2P, StarPointerGuidance, Appear2P);
    NEW_NERVE(HostTypeNrvWait2P, StarPointerGuidance, Wait2P);
    NEW_NERVE(HostTypeNrvEnd2P, StarPointerGuidance, End2P);
    NEW_NERVE(HostTypeNrvEndWait, StarPointerGuidance, EndWait);
};  // namespace NrvStarPointerGuidance

StarPointerGuidance::StarPointerGuidance(const char* pName)
    : LayoutActor(pName, true), mSpineFrame1P(nullptr), mSpineGuidance(nullptr), mSpineFrame2P(nullptr), mOutScreenTime(0), mRequestTime1P(0),
      mRequestTime2P(0), mGuidanceMessage(nullptr), mPrevGuidanceMessage(nullptr), mIsActiveRequest1P(false), mIsActiveRequest2P(false),
      mIsActive(true), mIsGuidanceDisabled(false), mTextLineNumGuidance(1), mTextLineNum1P(1), mTextLineNum2P(1) {
}

void StarPointerGuidance::init(const JMapInfoIter& rIter) {
    initLayoutManager("PointerGuidance", 1);
    MR::createAndAddPaneCtrl(this, "NPointerGuide", 1);
    MR::createAndAddPaneCtrl(this, "PointerFrame1", 1);
    MR::createAndAddPaneCtrl(this, "PointerFrame2", 1);

    mSpineFrame1P = new Spine(this, &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
    mSpineGuidance = new Spine(this, &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
    mSpineFrame2P = new Spine(this, &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);

    appear();
    MR::hidePaneRecursive(this, "NPointerGuide");
    MR::hidePaneRecursive(this, "PointerFrame1");
    MR::hidePaneRecursive(this, "PointerFrame2");

    mTextLineNum1P = MR::getTextLineNumMaxRecursive(this, "PointerFrame1");
    mTextLineNum2P = MR::getTextLineNumMaxRecursive(this, "PointerFrame2");
}

void StarPointerGuidance::control() {
    mSpineFrame1P->update();
    mSpineGuidance->update();
    mSpineFrame2P->update();

    if (!mIsActive) {
        mIsActiveRequest1P = false;
        mIsActiveRequest2P = false;
    }

    checkRequest2P();
    checkRequest1P();
}

void StarPointerGuidance::checkRequest2P() {
    if (!mIsActiveRequest2P) {
        if (mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvWait2P::sInstance) {
            mSpineFrame2P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEnd2P::sInstance);
        }

        mRequestTime2P = 0;
    } else {
        mRequestTime2P++;

        if (!MR::isStarPointerInScreen(WPAD_CHAN1)) {
            if (mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvEnd2P::sInstance ||
                mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance) {
                mSpineFrame2P->setNerve(&NrvStarPointerGuidance::HostTypeNrvAppear2P::sInstance);

                if (mRequestTime2P == 1) {
                    MR::startCSSound2P("CS_CAN_PLAY_2P", nullptr);
                } else {
                    MR::startCSSound2P("CS_OUT_DISPLAY_2P", nullptr);
                }
            }
        } else if (mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvWait2P::sInstance) {
            mSpineFrame2P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEnd2P::sInstance);
            MR::startCSSound2P("CS_IN_DISPLAY_2P", nullptr);
        }

        mIsActiveRequest2P = false;
    }
}

void StarPointerGuidance::checkRequest1P() {
    if (!mIsActiveRequest1P) {
        if (mRequestTime1P < 0) {
            mRequestTime1P--;
        } else {
            mRequestTime1P = -1;
        }

        mPrevGuidanceMessage = mGuidanceMessage;
        mGuidanceMessage = nullptr;
        if (mRequestTime1P > -::hNotRequestEndTime) {
            return;
        }

        if (mSpineFrame1P->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvAppear1P::sInstance) ||
            mSpineFrame1P->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvWait1P::sInstance)) {
            mSpineFrame1P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEnd1P::sInstance);
        }

        if (mSpineGuidance->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvWaitBlueStarGuide::sInstance)) {
            mSpineGuidance->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndBlueStarGuide::sInstance);
        }

        return;
    }

    if (mRequestTime1P < 0) {
        mRequestTime1P = 0;
    }
    mRequestTime1P++;

    if (!MR::isStarPointerInScreen(WPAD_CHAN0)) {
        if (mOutScreenTime > ::hOutScreenTime || mRequestTime1P == 1) {
            if (mSpineGuidance->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvWaitBlueStarGuide::sInstance)) {
                mSpineGuidance->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndBlueStarGuide::sInstance);
            }

            if (mSpineFrame1P->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvEnd1P::sInstance) ||
                mSpineFrame1P->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance)) {
                mSpineFrame1P->setNerve(&NrvStarPointerGuidance::HostTypeNrvAppear1P::sInstance);
                MR::startCSSound("CS_NOTICE_USE_DPD", "SE_SY_CS_NOTICE_USE_DPD", WPAD_CHAN0);
            }
        } else {
            mOutScreenTime++;
        }

    } else {
        if (mGuidanceMessage != nullptr) {
            if (mGuidanceMessage != mPrevGuidanceMessage) {
                mRequestTime1P = 0;
            }

            if (mRequestTime1P > ::hRequestTime) {
                if (mSpineGuidance->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance)) {
                    mTextLineNumGuidance = MR::countMessageLine(mGuidanceMessage);
                    MR::setTextBoxMessageRecursive(this, "NPointerGuide", mGuidanceMessage);
                    mSpineGuidance->setNerve(&NrvStarPointerGuidance::HostTypeNrvAppearBlueStarGuide::sInstance);
                }
            }

        } else {
            if (mSpineGuidance->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvWaitBlueStarGuide::sInstance)) {
                MR::clearTextBoxMessageRecursive(this, "NPointerGuide");
                mSpineGuidance->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndBlueStarGuide::sInstance);
            }
        }

        if (mSpineFrame1P->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvAppear1P::sInstance) ||
            mSpineFrame1P->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvWait1P::sInstance)) {
            mSpineFrame1P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEnd1P::sInstance);
        }

        mOutScreenTime = 0;
    }
    mIsActiveRequest1P = false;
    mPrevGuidanceMessage = mGuidanceMessage;
}

void StarPointerGuidance::exeAppear1P() {
    exeAppearCore(mSpineFrame1P, "PointerFrame1", mTextLineNum1P, &NrvStarPointerGuidance::HostTypeNrvWait1P::sInstance);
}

void StarPointerGuidance::exeWait1P() {
    exeWaitCore(mSpineFrame1P, "PointerFrame1", mTextLineNum1P);
}

void StarPointerGuidance::exeEnd1P() {
    exeEndCore(mSpineFrame1P, "PointerFrame1", mTextLineNum1P);
}

void StarPointerGuidance::exeAppearBlueStarGuide() {
    exeAppearCore(mSpineGuidance, "NPointerGuide", mTextLineNumGuidance, &NrvStarPointerGuidance::HostTypeNrvWaitBlueStarGuide::sInstance);
}

void StarPointerGuidance::exeWaitBlueStarGuide() {
    exeWaitCore(mSpineGuidance, "NPointerGuide", mTextLineNumGuidance);
}

void StarPointerGuidance::exeEndBlueStarGuide() {
    exeEndCore(mSpineGuidance, "NPointerGuide", mTextLineNumGuidance);
}

void StarPointerGuidance::exeAppear2P() {
    exeAppearCore(mSpineFrame2P, "PointerFrame2", mTextLineNum2P, &NrvStarPointerGuidance::HostTypeNrvWait2P::sInstance);
}

void StarPointerGuidance::exeWait2P() {
    exeWaitCore(mSpineFrame2P, "PointerFrame2", mTextLineNum2P);
}

void StarPointerGuidance::exeEnd2P() {
    exeEndCore(mSpineFrame2P, "PointerFrame2", mTextLineNum2P);
}

void StarPointerGuidance::exeEndWait() {
}

s32 StarPointerGuidance::exeAppearCore(Spine* pSpine, const char* pPaneName, u32 numLines, const Nerve* pNerve) {
    if (pSpine->mStep == 0) {
        MR::showPaneRecursive(this, pPaneName);

        if (numLines >= 2) {
            MR::startPaneAnim(this, pPaneName, "AppearTwoLine", 0);
        } else {
            MR::startPaneAnim(this, pPaneName, "AppearOneLine", 0);
        }

        return 1;
    }

    if (MR::isPaneAnimStopped(this, pPaneName, 0)) {
        pSpine->setNerve(pNerve);

        return 2;
    }

    return 0;
}

s32 StarPointerGuidance::exeWaitCore(Spine* pSpine, const char* pPaneName, u32 numLines) {
    if (pSpine->mStep == 0) {
        mIsGuidanceDisabled = false;

        MR::startPaneAnim(this, pPaneName, "Wait", 0);

        return 1;
    }

    if (mIsTimeOutEnabled && pSpine == mSpineGuidance && mSpineGuidance->mStep == ::hEndTimeOutHide) {
        mIsGuidanceDisabled = true;

        if (numLines >= 2) {
            MR::startPaneAnim(this, pPaneName, "EndTwoLine", 0);
        } else {
            MR::startPaneAnim(this, pPaneName, "EndOneLine", 0);
        }
    }

    return 0;
}

s32 StarPointerGuidance::exeEndCore(Spine* pSpine, const char* pPaneName, u32 numLines) {
    if (pSpine->mStep == 0) {
        if (pSpine == mSpineGuidance && mIsGuidanceDisabled) {
            return 1;
        }

        if (numLines >= 2) {
            MR::startPaneAnim(this, pPaneName, "EndTwoLine", 0);
        } else {
            MR::startPaneAnim(this, pPaneName, "EndOneLine", 0);
        }

        return 1;
    }

    if (MR::isPaneAnimStopped(this, pPaneName, 0)) {
        MR::hidePaneRecursive(this, pPaneName);
        pSpine->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);

        return 2;
    }

    return 0;
}

bool StarPointerGuidance::request1PGuidance(const char* pMessageId, bool enableTimeOut) {
    if (!MR::isConnectedWPad(WPAD_CHAN0) || !MR::isStarPointerValid(WPAD_CHAN0) && !MR::isStarPointerModeMarioLauncher()) {
        return false;
    }

    mGuidanceMessage = pMessageId != nullptr ? MR::getLayoutMessageDirect(pMessageId) : nullptr;
    mIsActiveRequest1P = true;
    mIsTimeOutEnabled = enableTimeOut;

    return true;
}

void StarPointerGuidance::tryResetTimeout() {
    if (!mIsTimeOutEnabled) {
        return;
    }

    if (!mIsGuidanceDisabled) {
        return;
    }

    mSpineGuidance->setNerve(&NrvStarPointerGuidance::HostTypeNrvWaitBlueStarGuide::sInstance);
}

bool StarPointerGuidance::isExistGuidanceOrFrame() {
    return isExistGuidance() || isExistFrame1P() || isExistFrame2P();
}

bool StarPointerGuidance::isExistGuidance() {
    if (mSpineGuidance->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance)) {
        return false;
    }

    return !mIsGuidanceDisabled;
}

bool StarPointerGuidance::isExistFrame1P() {
    return !mSpineFrame1P->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
}

bool StarPointerGuidance::isExistFrame2P() {
    return !mSpineFrame2P->isCurrentNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
}

void StarPointerGuidance::deactive() {
    mIsActive = false;

    MR::hidePaneRecursive(this, "PointerFrame1");
    MR::hidePaneRecursive(this, "NPointerGuide");
    MR::hidePaneRecursive(this, "PointerFrame2");
    mSpineFrame1P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
    mSpineGuidance->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
    mSpineFrame2P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);

    mPrevGuidanceMessage = nullptr;
    mGuidanceMessage = nullptr;
    mIsActiveRequest1P = false;
    mIsActiveRequest2P = false;
}
