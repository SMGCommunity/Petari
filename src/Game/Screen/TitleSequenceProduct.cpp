#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/EncouragePal60Window.hpp"
#include "Game/Screen/TitleSequenceProduct.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "Game/Util/TriggerChecker.hpp"

namespace TitleSequenceProductSub {
    LogoLayout::LogoLayout() :
        SimpleLayout("ロゴ", "TitleLogo", 2, -1)
    {
        initEffectKeeper(1, "TitleLogo", NULL);
        kill();
    }
};

namespace {
    NEW_NERVE(TitleSequenceProductDisplayEncouragePal60Window, TitleSequenceProduct, DisplayEncouragePal60Window);
    NEW_NERVE(TitleSequenceProductBgmPrepare, TitleSequenceProduct, BgmPrepare);
    NEW_NERVE(TitleSequenceProductLogoFadein, TitleSequenceProduct, LogoFadein);
    NEW_NERVE(TitleSequenceProductLogoWait, TitleSequenceProduct, LogoWait);
    NEW_NERVE(TitleSequenceProductLogoDisplay, TitleSequenceProduct, LogoDisplay);
    NEW_NERVE(TitleSequenceProductDecide, TitleSequenceProduct, Decide);
    NEW_NERVE(TitleSequenceProductDead, TitleSequenceProduct, Dead);
};

TitleSequenceProduct::TitleSequenceProduct() :
    NerveExecutor(""),
    mLogoLayout(NULL),
    mPressStartLayout(NULL),
    mEncouragePal60Window(NULL),
    mAButtonChecker(NULL),
    mBButtonChecker(NULL)
{
    if (MR::isDisplayEncouragePal60Window()) {
        initNerve(&TitleSequenceProductDisplayEncouragePal60Window::sInstance);
    }
    else {
        initNerve(&TitleSequenceProductBgmPrepare::sInstance);
    }

    mLogoLayout = new TitleSequenceProductSub::LogoLayout();
    mPressStartLayout = new SimpleLayout("Press[A][B]", "PressStart", 1, -1);
    mPressStartLayout->initWithoutIter();
    mAButtonChecker = new TriggerChecker();
    mBButtonChecker = new TriggerChecker();
    mIsDisplayEncouragePal60Window = MR::isDisplayEncouragePal60Window();
    mEncouragePal60Window = new EncouragePal60Window();
    mEncouragePal60Window->initWithoutIter();
    mEncouragePal60Window->kill();
}

// FIXME: GPR4 and GPR5 are swapped and missing addi instruction.
void TitleSequenceProduct::exeDisplayEncouragePal60Window() {
    if (MR::isFirstStep(this)) {
        OSTime timeAnnounced = GameDataFunction::getSysConfigFileTimeAnnounced();
        OSTime ticks = OSGetTime();
        s32 day = OSTicksToSeconds(ticks) / (60 * 60 * 24);
        s32 dayAnnounced = OSTicksToSeconds(timeAnnounced) / (60 * 60 * 24);

        if (timeAnnounced == 0 || ticks < timeAnnounced || dayAnnounced < day) {
            mEncouragePal60Window->appear();
            GameDataFunction::updateSysConfigFileTimeAnnounced();
        }
    }

    if (MR::isDead(mEncouragePal60Window)) {
        setNerve(&TitleSequenceProductBgmPrepare::sInstance);
    }
}

void TitleSequenceProduct::exeBgmPrepare() {
    if (MR::isFirstStep(this)) {
        MR::startStageBGM("STM_TITLE", true);
    }

    if (MR::isPreparedStageBgm()) {
        setNerve(&TitleSequenceProductLogoFadein::sInstance);
    }
}

void TitleSequenceProduct::exeLogoFadein() {
    if (MR::isFirstStep(this)) {
        mLogoLayout->appear();
        MR::startAnim(mLogoLayout, "Appear", 0);
        MR::unlockStageBGM();
    }

    if (MR::isAnimStopped(mLogoLayout, 0)) {
        setNerve(&TitleSequenceProductLogoWait::sInstance);
    }
}

void TitleSequenceProduct::exeLogoWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(mLogoLayout, "Wait", 0);
        MR::emitEffect(mLogoLayout, "TitleLogoLight");
        MR::emitEffect(mLogoLayout, "TitleLogoLightB");
        MR::emitEffect(mLogoLayout, "TitleLogoLightC");
        MR::emitEffect(mLogoLayout, "TitleLogoLightD");
        MR::emitEffect(mLogoLayout, "TitleLogoLightE");
        MR::emitEffect(mLogoLayout, "TitleLogoLightF");
        MR::emitEffect(mLogoLayout, "TitleLogoLightG");
    }

    if (MR::isStep(this, 25)) {
        setNerve(&TitleSequenceProductLogoDisplay::sInstance);
    }
}

void TitleSequenceProduct::exeLogoDisplay() {
    if (MR::isFirstStep(this)) {
        mPressStartLayout->appear();
        MR::startAnim(mPressStartLayout, "Appear", 0);
    }

    if (MR::isAnimStopped(mPressStartLayout, 0)) {
        MR::startAnim(mPressStartLayout, "Wait", 0);
    }

    mAButtonChecker->update(MR::testCorePadButtonA(0));
    mBButtonChecker->update(MR::testCorePadButtonB(0));

    if (mAButtonChecker->getLevel() && mBButtonChecker->getLevel()) {
        MR::stopStageBGM(75);
        MR::startSystemSE("SE_SY_GAME_START", -1, -1);
        MR::startCSSound("CS_CLICK_CLOSE", 0, 0);
        MR::tryRumblePadMiddle(this, 0);
        setNerve(&TitleSequenceProductDecide::sInstance);
    }
    else {
        updateButtonReaction(mAButtonChecker, "ReactionA");
        updateButtonReaction(mBButtonChecker, "ReactionB");
        updatePressStartReaction();
    }
}

void TitleSequenceProduct::exeDecide() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(mLogoLayout, "Decide", 0);
        MR::deleteEffectAll(mLogoLayout);
        MR::startAnim(mPressStartLayout, "End", 0);
    }

    if (MR::isAnimStopped(mLogoLayout, 0) && MR::isAnimStopped(mPressStartLayout, 0)) {
        setNerve(&TitleSequenceProductDead::sInstance);
    }
}

void TitleSequenceProduct::exeDead() {
    if (MR::isFirstStep(this)) {
        mLogoLayout->kill();
        mPressStartLayout->kill();

        if (mIsDisplayEncouragePal60Window) {
            mEncouragePal60Window->kill();
        }
    }
}

void TitleSequenceProduct::appear() {
    if (mIsDisplayEncouragePal60Window) {
        setNerve(&TitleSequenceProductDisplayEncouragePal60Window::sInstance);
    }
    else {
        setNerve(&TitleSequenceProductBgmPrepare::sInstance);
    }
}

void TitleSequenceProduct::kill() {
    setNerve(&TitleSequenceProductDead::sInstance);
}

bool TitleSequenceProduct::isActive() const {
    return !isNerve(&TitleSequenceProductDead::sInstance);
}

void TitleSequenceProduct::updateButtonReaction(TriggerChecker* pButtonChecker, const char* pAnimName) {
    if (pButtonChecker->getOnTrigger()) {
        MR::startAnim(mLogoLayout, pAnimName, 1);
        MR::setAnimFrameAndStop(mLogoLayout, 0.0f, 1);
    }
    else if (pButtonChecker->getOffTrigger()) {
        MR::startAnim(mLogoLayout, pAnimName, 1);
    }
}

void TitleSequenceProduct::updatePressStartReaction() {
    if (mAButtonChecker->getOnTrigger()) {
        MR::startAnim(mPressStartLayout, "ButtonReaction", 0);
    }
    else if (mBButtonChecker->getOnTrigger()) {
        MR::startAnim(mPressStartLayout, "ButtonReaction", 0);
    }
}
