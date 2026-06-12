#include "Game/Screen/StarPointerLayout.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/LayoutCoreUtil.hpp"
#include "Game/Screen/StarPointerBlur.hpp"
#include "Game/Screen/StarPointerCommandStream.hpp"
#include "Game/Screen/StarPointerController.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPadPointer.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution/wpad.h>

namespace {
    // TODO: names
    static GXColor sColorA[] = {
        {0x00, 0x32, 0xFF, 0xFF},
        {0xFF, 0xFF, 0x00, 0xFF},
        {0x55, 0xFF, 0x00, 0xFF},
        {0x7F, 0x7F, 0x00, 0xFF},
    };

    static GXColor sColorB[] = {
        {0x00, 0x0A, 0x32, 0xFF},
        {0x32, 0x32, 0x00, 0xFF},
        {0x00, 0xFF, 0x00, 0xFF},
        {0x7F, 0x7F, 0x00, 0xFF},
    };

    static const s32 sTouchCount = 1;
    static const f32 sNormalRadius = 15.0f;
    static const f32 sHoldRadius = 45.0f;
    static const f32 sHandRadius = 20.0f;

    // static const _32 sStepRadian =
    static const s32 hGripTime = 20;
    static const s32 hReleaseTime = 30;
    static const s32 hHideRumbleTime = 30;
    // static const _32 hInvalidateWaitFrame =
    // static const _32 sStarVelZeroRange =
    // static const _32 sStarRateScale =
    // static const _32 sStarRateMax =
    // static const _32 h16x9ScaleX =
    static const f32 sRotateMult = 900.0f;
    // static const f32 sRotateHandMult =
    static const f32 hEndAnimRate = 2.0f;
    static const s32 sBeginHideTime = 0;
    static const f32 sDegreeMin = 0.0f;
};  // namespace

namespace NrvStarPointerLayout {
    NEW_NERVE(HostTypeNrvWait, StarPointerLayout, Wait);
    NEW_NERVE_ONEND(HostTypeNrvOnReaction, StarPointerLayout, OnReaction, OnReaction);
    NEW_NERVE(HostTypeNrvGrip, StarPointerLayout, Grip);
    NEW_NERVE_ONEND(HostTypeNrvHold, StarPointerLayout, Hold, Hold);
    NEW_NERVE_ONEND(HostTypeNrvHoldTouch, StarPointerLayout, Hold, Hold);
    NEW_NERVE(HostTypeNrvRelease, StarPointerLayout, Release);
    NEW_NERVE(HostTypeNrvOutScreen, StarPointerLayout, OutScreen);
    NEW_NERVE_ONEND(HostTypeNrvNotConnected, StarPointerLayout, Hide, Hide);
    NEW_NERVE(HostTypeNrvHide, StarPointerLayout, Hide);
    NEW_NERVE(HostTypeNrvHideImmediately, StarPointerLayout, Hide);
    NEW_NERVE_ONEND(HostTypeNrvInvalidate, StarPointerLayout, Hide, Hide);
    NEW_NERVE_ONEND(HostTypeNrvInvalidateWait, StarPointerLayout, Hide, Hide);
};  // namespace NrvStarPointerLayout

StarPointerLayout::StarPointerLayout(const char* pName)
    : LayoutActor(pName, true), mPosition(0.0f, 0.0f), mPointerKind(StarPointerKind_NULL), mAnimType(AnimType_HandPaa), mTouchTimer(0),
      mPadChannel(-1), mRadius(::sNormalRadius), _3C(0.0f), mRotateAngle(0.0f), mDirector(nullptr), mBlur(nullptr), mCommandStream(nullptr),
      mNumber(nullptr), mActor(nullptr), mNewTouchedID(0), mTouchedID(0), mStartTouch(false), mStartDisableShoot(false), mSingleTouch(false),
      mShootDisabled(false), mIsPointerValid(true), mIsAppear(true), mAppearTime(0) {
}

void StarPointerLayout::initWithPort(s32 channel) {
    mPadChannel = channel;
    if (mPadChannel == WPAD_CHAN1) {
        initLayoutManager("DPDPointerSub", 2);
    } else {
        initLayoutManager("DPDPointer", 1);
    }

    MR::createAndAddPaneCtrl(this, "StarPointer", 1);
    MR::createAndAddPaneCtrl(this, "HandPointer", 1);
    MR::createAndAddPaneCtrl(this, "StarFrame", 1);
    MR::createAndAddPaneCtrl(this, "Star", 1);
    if (mPadChannel == WPAD_CHAN0) {
        MR::createAndAddPaneCtrl(this, "PicNozzle", 1);
    }
    MR::createAndAddGroupCtrl(this, "GroupRing", 1);

    initNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance);

    mBlur = new StarPointerBlur(new JUTTexture(MR::loadTexFromArc("StarPointerBlur.arc", "Blur.bti"), 0));

    mCommandStream = new StarPointerCommandStream(&mPosition);

    mBlur->setPortAndColor(mPadChannel, &::sColorA[mPadChannel], &::sColorB[mPadChannel]);
    mCommandStream->setPortAndColor(mPadChannel, &::sColorA[mPadChannel]);
    mCommandStream->initWithoutIter();

    mNumber = new StarPointerNumber(this, mPadChannel, "スターポインタナンバー");
    mNumber->initWithoutIter();
    mNumber->appear();

    changeLayout(StarPointerKind_StarPointer);
    hideAll();
    appear();
}

void StarPointerLayout::appear() {
    LayoutActor::appear();
    setNerve(&NrvStarPointerLayout::HostTypeNrvWait::sInstance);
}

void StarPointerLayout::kill() {
    LayoutActor::kill();
}

bool StarPointerLayout::startCommandStream(const LiveActor* pActor, const TVec3f* pPos, bool b) {
    if (mCommandStream->mWorldPos != nullptr) {
        return false;
    }

    mCommandStream->start(pPos, b);
    hideBlur();
    mActor = pActor;
    MR::startStarPointerModeCommandStream(this);
    return true;
}

void StarPointerLayout::endCommandStream(const LiveActor* pActor) {
    forceEndCommandStream();
    MR::endStarPointerMode(this);
}

void StarPointerLayout::forceEndCommandStream() {
    mCommandStream->hide();
    mCommandStream->clear();
    mBlur->show();
    mActor = nullptr;
}

bool StarPointerLayout::isCommandStream(const LiveActor* pActor) {
    return mCommandStream->mWorldPos != nullptr && mActor == pActor;
}

void StarPointerLayout::exeWait() {
    if (MR::isFirstStep(this)) {
        if (mPointerKind == StarPointerKind_StarPointer) {
            startAnimStarPointer("Wait");
        } else if (mPointerKind == StarPointerKind_HandPointer || mPointerKind == StarPointerKind_2 ||
                   mPointerKind == StarPointerKind_HandPointerReactionWithCrossCursor) {
            changeToHandPointer();
            startAnimHandPaa();
        } else if (mPointerKind == StarPointerKind_FingerPointer) {
            startAnimHandFinger();
        } else if (mPointerKind == StarPointerKind_5) {
            startAnimStarPointer("Wait");
        }

        if (mPointerKind == StarPointerKind_StarPointerNozzle) {
            MR::showPane(this, "PicNozzle");
            MR::stopPaneAnim(this, "PicNozzle", 0);
            startAnimStarPointer("Wait");

        } else if (mPadChannel == WPAD_CHAN0) {
            MR::hidePane(this, "PicNozzle");
        }
    }

    if (isTouch()) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvOnReaction::sInstance);
    } else {
        if (MR::testCorePadButtonA(mPadChannel)) {
            setNerve(&NrvStarPointerLayout::HostTypeNrvGrip::sInstance);
        }
    }
}

void StarPointerLayout::exeGrip() {
    if (MR::isFirstStep(this)) {
        if (mPadChannel == WPAD_CHAN1 && isAppear()) {
            MR::startSystemSE("SE_SY_DPD_B_TRIG");
        }

        if (mPointerKind == StarPointerKind_StarPointer) {
            if (mPadChannel == WPAD_CHAN1) {
                startAnimStarPointer("Grip");
            } else {
                if (mCommandStream->mWorldPos != nullptr) {
                    mCommandStream->show();
                    startAnimHandGuu();
                } else {
                    startAnimStarPointer("Wait");
                }
            }
        } else if (mPointerKind == StarPointerKind_HandPointer) {
            startAnimHandGuu();
        } else if (mPointerKind == StarPointerKind_2 || mPointerKind == StarPointerKind_HandPointerReactionWithCrossCursor) {
            if (mAnimType != AnimType_Finger) {
                startAnimHandGuu();
            }
        } else if (mPointerKind == StarPointerKind_FingerPointer) {
            startAnimHandFinger();
        } else if (mPointerKind == StarPointerKind_5) {
            startAnimHandPaa();
        }

        if (mPointerKind == StarPointerKind_StarPointerNozzle) {
            MR::showPane(this, "PicNozzle");
            MR::startPaneAnim(this, "PicNozzle", "Touch", 0);
            startAnimStarPointer("Touch");
        } else {
            if (mPadChannel == WPAD_CHAN0) {
                MR::hidePane(this, "PicNozzle");
            }
        }
    }

    if (mPointerKind == StarPointerKind_2 && mAnimType == AnimType_Finger && !isTouch()) {
        startAnimHandGuu();
    }

    if (!MR::testCorePadButtonA(mPadChannel)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvRelease::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, ::hGripTime) || MR::isHiddenPane(this, "StarPointer")) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvHold::sInstance);
    }
}

void StarPointerLayout::exeHold() {
    if (MR::isFirstStep(this)) {
        if (mPointerKind == StarPointerKind_StarPointer) {
            if (mPadChannel == WPAD_CHAN1) {
                startAnimStarPointer("Hold");
                mRadius = ::sHoldRadius;
            } else {
                if (mCommandStream->mWorldPos != nullptr) {
                    mCommandStream->show();
                    startAnimHandGuu();
                } else {
                    startAnimStarPointer("Wait");
                }
            }
        } else if (mPointerKind == StarPointerKind_HandPointer) {
            startAnimHandGuu();
            mRadius = ::sHandRadius;
        } else if (mPointerKind == StarPointerKind_2 || mPointerKind == StarPointerKind_HandPointerReactionWithCrossCursor) {
            if (mAnimType != AnimType_Finger) {
                startAnimHandGuu();
            }
            mRadius = ::sHandRadius;
        } else if (mPointerKind == StarPointerKind_FingerPointer) {
            startAnimHandFinger();
        } else if (mPointerKind == StarPointerKind_5) {
            startAnimHandPaa();
        }

        if (mPointerKind == StarPointerKind_StarPointerNozzle) {
            MR::showPane(this, "PicNozzle");
            if (MR::isPaneAnimStopped(this, "PicNozzle", 0)) {
                MR::startPaneAnim(this, "PicNozzle", "Touch", 0);
            }
            startAnimStarPointer("Touch");
        } else {
            if (mPadChannel == WPAD_CHAN0) {
                MR::hidePane(this, "PicNozzle");
            }
        }
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvHold::sInstance)) {
        if (isTouch()) {
            setNerve(&NrvStarPointerLayout::HostTypeNrvHoldTouch::sInstance);
            return;
        }

        if (mPointerKind == StarPointerKind_2 ||
            (mPointerKind == StarPointerKind_HandPointerReactionWithCrossCursor && mAnimType == AnimType_Finger)) {
            startAnimHandGuu();
        }
    } else {
        if (!isTouch()) {
            setNerve(&NrvStarPointerLayout::HostTypeNrvHold::sInstance);
            return;
        }
    }

    if (!MR::testCorePadButtonA(mPadChannel)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvRelease::sInstance);
    }
}

void StarPointerLayout::endHold() {
    tearDownHold();
}

void StarPointerLayout::tearDownHold() {
    if (mPointerKind == StarPointerKind_StarPointer && mPadChannel == WPAD_CHAN1) {
        if (isNerve(&NrvStarPointerLayout::HostTypeNrvHold::sInstance) || isNerve(&NrvStarPointerLayout::HostTypeNrvHoldTouch::sInstance)) {
            mRadius = ::sHandRadius;
        } else {
            mRadius = ::sNormalRadius;
        }
    } else if (mPointerKind == StarPointerKind_HandPointer || mPointerKind == StarPointerKind_2) {
        mRadius = ::sHoldRadius;
    }
}

void StarPointerLayout::exeRelease() {
    if (isTouch()) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvOnReaction::sInstance);
        return;
    }

    if (MR::isFirstStep(this)) {
        if (mPadChannel == WPAD_CHAN1 && isAppear()) {
            MR::startSystemSE("SE_SY_DPD_B_RELEASE");
        }

        if (mPointerKind == StarPointerKind_StarPointer) {
            if (mPadChannel == WPAD_CHAN1) {
                startAnimStarPointer("Release");
            } else {
                changeToStarPointer();
            }
        } else if (mPointerKind == StarPointerKind_HandPointer || mPointerKind == StarPointerKind_2 ||
                   mPointerKind == StarPointerKind_HandPointerReactionWithCrossCursor) {
            startAnimHandPaa();
        } else if (mPointerKind == StarPointerKind_FingerPointer) {
            startAnimHandFinger();
        } else if (mPointerKind == StarPointerKind_5) {
            startAnimStarPointer("Wait");
        }

        if (mPointerKind == StarPointerKind_StarPointerNozzle) {
            MR::showPane(this, "PicNozzle");
            MR::stopPaneAnim(this, "PicNozzle", 0);
            startAnimStarPointer("Wait");
        } else if (mPadChannel == WPAD_CHAN0) {
            MR::hidePane(this, "PicNozzle");
        }
    }

    if (MR::testCorePadButtonA(mPadChannel)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvGrip::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, ::hReleaseTime)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvWait::sInstance);
    }
}

void StarPointerLayout::exeOnReaction() {
    if (MR::isFirstStep(this)) {
        if (mPadChannel == WPAD_CHAN0 && mPointerKind == StarPointerKind_StarPointer) {
            startAnimHandPaa();
        } else {
            if (mPointerKind == StarPointerKind_StarPointer) {
                startAnimStarPointer("Touch");
            } else if (mPointerKind == StarPointerKind_HandPointer) {
                startAnimHandPaa();
            } else if (mPointerKind == StarPointerKind_2 || mPointerKind == StarPointerKind_HandPointerReactionWithCrossCursor) {
                startAnimHandFinger();
            } else if (mPointerKind == StarPointerKind_FingerPointer) {
                startAnimHandFinger();
            } else if (mPointerKind == StarPointerKind_5) {
                startAnimStarPointer("Touch");
            }
        }

        if (mPointerKind == StarPointerKind_StarPointerNozzle) {
            MR::showPane(this, "PicNozzle");
            MR::stopPaneAnim(this, "PicNozzle", 0);
            startAnimStarPointer("Wait");
        } else if (mPadChannel == WPAD_CHAN0) {
            MR::hidePane(this, "PicNozzle");
        }
    }

    if (MR::testCorePadButtonA(mPadChannel)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvGrip::sInstance);
        return;
    }

    if (!isTouch()) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvWait::sInstance);
    }
}

void StarPointerLayout::endOnReaction() {
}

void StarPointerLayout::updateTouch() {
    mIsNewTouch = false;

    mSingleTouch = mStartSingleTouch;
    mStartSingleTouch = false;

    if (mStartTouch) {
        if (mTouchTimer == 0 || mNewTouchedID != mTouchedID) {
            mIsNewTouch = true;
            mTouchedID = mNewTouchedID;
        }
        mTouchTimer = ::sTouchCount;
        mStartTouch = false;
        return;
    }

    if (mTouchTimer > 0) {
        mTouchTimer--;
    }
}

bool StarPointerLayout::isTouch() {
    return mTouchTimer > 0;
}

void StarPointerLayout::exeOutScreen() {
    if (MR::isFirstStep(this)) {
        hideBlur();
        if (mPadChannel == WPAD_CHAN0) {
            MR::hidePane(this, "PicNozzle");
        }
    }

    if (MR::isStarPointerInScreen(mPadChannel)) {
        if (MR::testCorePadButtonA(mPadChannel)) {
            setNerve(&NrvStarPointerLayout::HostTypeNrvGrip::sInstance);
        } else {
            setNerve(&NrvStarPointerLayout::HostTypeNrvWait::sInstance);
        }
        return;
    }

    if (mCommandStream->mWorldPos != nullptr && MR::testCorePadButtonA(mPadChannel)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvOutScreen::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, ::sBeginHideTime)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvHide::sInstance);
    }
}

void StarPointerLayout::hideAll() {
    hideBlur();
    mCommandStream->hide();
    if (mPadChannel == WPAD_CHAN0) {
        MR::hidePane(this, "PicNozzle");
        MR::hidePane(this, "HandArrow");
    }
    MR::hidePaneRecursive(this, "HandPointer");
    MR::hidePaneRecursive(this, "StarPointer");
}

void StarPointerLayout::exeHide() {
    if (MR::isFirstStep(this)) {
        if (mPadChannel == WPAD_CHAN0) {
            MR::hidePane(this, "PicNozzle");
        }

        if (isNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance)) {
            MR::hidePaneRecursive(this, "HandPointer");
            MR::hidePaneRecursive(this, "StarPointer");
        } else if (isNerve(&NrvStarPointerLayout::HostTypeNrvNotConnected::sInstance) ||
                   isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidate::sInstance) ||
                   isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidateWait::sInstance)) {
            MR::hideLayout(this);
            MR::stopAnimFrame(this);
            MR::offCalcAnim(this);
        } else {
            if (mPointerKind == StarPointerKind_StarPointer || mPointerKind == StarPointerKind_5 ||
                mPointerKind == StarPointerKind_StarPointerNozzle) {
                startAnimStarPointer("End");
                MR::setAnimRate(this, ::hEndAnimRate, 0);
            } else if (mPointerKind == StarPointerKind_HandPointer || mPointerKind == StarPointerKind_2 ||
                       mPointerKind == StarPointerKind_HandPointerReactionWithCrossCursor || mPointerKind == StarPointerKind_FingerPointer) {
                MR::hidePane(this, "HandPointer");
            }
        }

        hideBlur();
        mCommandStream->hide();
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvHide::sInstance) || isNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance)) {
        if (MR::isStarPointerInScreen(mPadChannel)) {
            setNerve(&NrvStarPointerLayout::HostTypeNrvWait::sInstance);
        }
    }
}

void StarPointerLayout::endHide() {
    tearDownHide();
}

void StarPointerLayout::tearDownHide() {
    if (mPointerKind == StarPointerKind_StarPointer || mPointerKind == StarPointerKind_5 || mPointerKind == StarPointerKind_StarPointerNozzle) {
        MR::setAnimRate(this, 1.0f, 0);
    } else if (mPointerKind == StarPointerKind_HandPointer || mPointerKind == StarPointerKind_2 || mPointerKind == StarPointerKind_FingerPointer) {
        MR::showPane(this, "HandPointer");
    }
    MR::showLayout(this);
    MR::releaseAnimFrame(this);
    MR::onCalcAnim(this);
    calcAnim();
}

void StarPointerLayout::calcAnim() {
    LayoutActor::calcAnim();
    mCommandStream->calcAnim();
    mNumber->calcAnim();
}

void StarPointerLayout::draw() const {
    if (!isAppear()) {
        mBlur->hideAll();
        return;
    }

    if (mPadChannel == WPAD_CHAN0 && MR::isStarPointer1PInvalid2PValidMode()) {
        mBlur->hideAll();
        return;
    }

    drawDecoration();
    LayoutActor::draw();
    mNumber->draw();
}

void StarPointerLayout::drawDecoration() const {
    if (mPointerKind == StarPointerKind_HandPointer || mPointerKind == StarPointerKind_2 || mPointerKind == StarPointerKind_FingerPointer ||
        (mPointerKind == StarPointerKind_5 && MR::testCorePadButtonA(mPadChannel))) {
        mBlur->hideAll();
        return;
    }

    MR::loadProjectionMtxFor2D();
    mBlur->draw();
    mCommandStream->draw();
}

void StarPointerLayout::control() {
    mAppearTime++;
    mNumber->movement();
    MR::convertPaneLocalPosToScreenPos(&mPosition, MR::getRootPane(this), TVec2f(0.0f, 0.0f));

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvNotConnected::sInstance)) {
        if (MR::isConnectedWPad(mPadChannel)) {
            hideAll();
            setNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance);
        }
        return;
    }

    if (!MR::isConnectedWPad(mPadChannel)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvNotConnected::sInstance);
        return;
    }

    if (mPadChannel == WPAD_CHAN0) {
        if (mPointerKind == StarPointerKind_HandPointerReactionWithCrossCursor &&
            (getAnimType() == AnimType_HandGuu || getAnimType() == AnimType_HandPaa) && !isNerve(&NrvStarPointerLayout::HostTypeNrvHide::sInstance) &&
            !isNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance) &&
            !isNerve(&NrvStarPointerLayout::HostTypeNrvOutScreen::sInstance)) {
            MR::showPane(this, "HandArrow");
        } else {
            MR::hidePane(this, "HandArrow");
        }
    }

    updateDecoration();
    updateTouch();

    mShootDisabled = mStartDisableShoot;
    mStartDisableShoot = false;

    if (!isAppear()) {
        if (!isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidate::sInstance)) {
            setNerve(&NrvStarPointerLayout::HostTypeNrvInvalidate::sInstance);
        }
        setPosition();
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidate::sInstance) || isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidateWait::sInstance)) {
        if (MR::isStarPointerInScreen(mPadChannel)) {
            setNerve(&NrvStarPointerLayout::HostTypeNrvWait::sInstance);
        } else {
            hideAll();
            setNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance);
        }
        setPosition();
        return;
    }

    if (!MR::isStarPointerInScreen(mPadChannel)) {
        StarPointerController* controller = StarPointerFunction::getStarPointerDirector()->getStarPointerController(mPadChannel);
        if (!isNerve(&NrvStarPointerLayout::HostTypeNrvOutScreen::sInstance) && !isNerve(&NrvStarPointerLayout::HostTypeNrvHide::sInstance) &&
            !isNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance)) {
            if (controller->isOutScreenLong()) {
                if (mPointerKind == StarPointerKind_StarPointer) {
                    if (MR::isHiddenPane(this, "StarPointer")) {
                        startAnimHandGuu();
                    } else {
                        startAnimStarPointer("Blink");
                    }
                } else if (mPointerKind == StarPointerKind_HandPointer) {
                    startAnimHandPaa();
                }
                setNerve(&NrvStarPointerLayout::HostTypeNrvOutScreen::sInstance);
                rotateTest();
                return;
            }
        } else {
            rotateTest();
            return;
        }
    }

    rotateTest();

    if (mPadChannel == WPAD_CHAN1) {
        MR::startAnim(this, "Transparency", 1);

        if (MR::isStarPointer2PTransparencyMode() && !isNerve(&NrvStarPointerLayout::HostTypeNrvHide::sInstance)) {
            mBlur->setPortAndColor(mPadChannel, &::sColorB[mPadChannel], &::sColorB[mPadChannel]);
            MR::setAnimFrameAndStop(this, 1.0f, 1);
        } else {
            mBlur->setPortAndColor(mPadChannel, &::sColorA[mPadChannel], &::sColorB[mPadChannel]);
            MR::setAnimFrameAndStop(this, 0.0f, 1);
        }
    }

    setPosition();
}

void StarPointerLayout::setPosition() {
    StarPointerController* controller = StarPointerFunction::getStarPointerDirector()->getStarPointerController(mPadChannel);
    TVec2f pos = controller->mPastInfo.mPos;
    setTrans(pos);
    mNumber->setTrans(pos);
}

void StarPointerLayout::rotateTest() {
    f32 angle = mRotateAngle;
    if (MR::isStarPointerInScreen(mPadChannel)) {
        TVec2f horiz;
        MR::getWPad(mPadChannel)->mPointer->getHorizonVec(&horiz);
        angle = MR::normalizeAbs(MR::toDegree(JMAATan2(horiz.y, horiz.x)), ::sDegreeMin, 180.0f);
        if (angle != 0.0f) {
            mRotateAngle = angle;
        }
    }

    if (mPointerKind == StarPointerKind_StarPointer) {
        angle *= ::sRotateMult;
        TVec3f rotate;
        MR::copyPaneRotate(&rotate, this, "StarPointer");
        MR::setPaneRotate(this, rotate.x, rotate.y, -angle, "StarPointer");
    } else if (mPointerKind == StarPointerKind_StarPointerNozzle && mPadChannel == WPAD_CHAN0) {
        TVec2f targetScreenPos;
        MR::calcScreenPosition(&targetScreenPos, StarPointerFunction::getStarPointerDirector()->mNozzleAimPos);
        TVec2f pointerPos = MR::getStarPointerScreenPositionOrEdge(mPadChannel);
        calcPoseNozzle(targetScreenPos, pointerPos);
    }
}

void StarPointerLayout::calcPoseNozzle(const TVec2f& rTargetPos, const TVec2f& rPointerPos) {
    TVec2f diff = rPointerPos;
    diff.sub(rTargetPos);

    if (MR::isScreen16Per9()) {
        // stripped
    }

    if (!MR::isNearZero(diff)) {
        f32 dist;
        MR::separateScalarAndDirection(&dist, &diff, diff);

        f32 angle = MR::toDegree(JMAATan2(diff.x, diff.y));
        TVec3f rotate;
        MR::copyPaneRotate(&rotate, this, "PicNozzle");
        MR::setPaneRotate(this, rotate.x, rotate.y, angle, "PicNozzle");
    }
}

void StarPointerLayout::changeLayout(StarPointerKind pointerKind) {
    if (pointerKind == mPointerKind) {
        return;
    }

    switch (pointerKind) {
    case StarPointerKind_StarPointer:
        if (mCommandStream->mWorldPos == nullptr) {
            mAnimType = AnimType_NULL;
        } else {
            mCommandStream->show();
        }
        mRadius = ::sNormalRadius;
        break;
    case StarPointerKind_HandPointer:
        mCommandStream->hide();
        mRadius = ::sHandRadius;
        break;
    case StarPointerKind_2:
        mCommandStream->hide();
        mRadius = ::sHandRadius;
        break;
    case StarPointerKind_HandPointerReactionWithCrossCursor:
        mCommandStream->hide();
        mRadius = ::sHandRadius;
        break;
    case StarPointerKind_FingerPointer:
        mCommandStream->hide();
        mRadius = ::sHandRadius;
        break;
    case StarPointerKind_5:
        mCommandStream->hide();
        mRadius = ::sHandRadius;
        break;
    case StarPointerKind_StarPointerNozzle:
        mCommandStream->hide();
        mRadius = ::sNormalRadius;
        break;
    }

    mPointerKind = pointerKind;

    if (!isNerve(&NrvStarPointerLayout::HostTypeNrvHide::sInstance) && !isNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance)) {
        calcAnim();
    }

    resetSameNerve();
}

bool StarPointerLayout::isChanceToRumble() const {
    if (mIsNewTouch) {
        return true;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvOnReaction::sInstance) && MR::isStep(this, 1)) {
        return true;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvGrip::sInstance) && MR::isStep(this, 1)) {
        return true;
    }

    return false;
}

void StarPointerLayout::hideBlur() {
    mBlur->hideAll();
}

void StarPointerLayout::resetSameNerve() {
    if (isNerve(&NrvStarPointerLayout::HostTypeNrvWait::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvWait::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvOnReaction::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvOnReaction::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvGrip::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvGrip::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvHold::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvHold::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvHoldTouch::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvHoldTouch::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvRelease::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvRelease::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvOutScreen::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvOutScreen::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvNotConnected::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvNotConnected::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidate::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvInvalidate::sInstance);
        return;
    }

    if (isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidateWait::sInstance)) {
        setNerve(&NrvStarPointerLayout::HostTypeNrvInvalidateWait::sInstance);
        return;
    }
}

void StarPointerLayout::changeToStarPointer() {
    if (mBlur->mBlurPoints[0].mIsHidden) {
        mBlur->show();
    }

    MR::hidePaneRecursive(this, "HandPointer");
    MR::showPaneRecursive(this, "StarPointer");
}

void StarPointerLayout::changeToHandPointer() {
    hideBlur();
    MR::hidePaneRecursive(this, "StarPointer");
    MR::showPaneRecursive(this, "HandPointer");

    if (mPadChannel == WPAD_CHAN0) {
        MR::showPaneRecursive(this, "PicPlayer1");
        MR::hidePaneRecursive(this, "PicPlayer2");
    } else {
        MR::showPaneRecursive(this, "PicPlayer2");
        MR::hidePaneRecursive(this, "PicPlayer1");
    }
}

void StarPointerLayout::startAnimStarPointer(const char* pAnimName) {
    changeToStarPointer();
    MR::startPaneAnim(this, "StarPointer", pAnimName, 0);
    mAnimType = AnimType_StarPointer;
}

void StarPointerLayout::startAnimHandPaa() {
    changeToHandPointer();
    MR::startPaneAnim(this, "HandPointer", "HandType", 0);
    MR::setPaneAnimFrameAndStop(this, "HandPointer", 2.0f, 0);
    mAnimType = AnimType_HandPaa;
}

void StarPointerLayout::startAnimHandGuu() {
    changeToHandPointer();
    MR::startPaneAnim(this, "HandPointer", "HandType", 0);
    MR::setPaneAnimFrameAndStop(this, "HandPointer", 1.0f, 0);
    mAnimType = AnimType_HandGuu;
}

void StarPointerLayout::startAnimHandFinger() {
    changeToHandPointer();
    MR::startPaneAnim(this, "HandPointer", "HandType", 0);
    MR::setPaneAnimFrameAndStop(this, "HandPointer", 0.0f, 0);
    mAnimType = AnimType_Finger;
}

void StarPointerLayout::updateDecoration() {
    if (mPointerKind == StarPointerKind_HandPointer || mPointerKind == StarPointerKind_2) {
        return;
    }

    if (mCommandStream->mWorldPos != nullptr) {
        mCommandStream->movement();
    } else {
        mBlur->movement();
    }
}

StarPointerNumber::StarPointerNumber(StarPointerLayout* pParent, s32 padChannel, const char* pName)
    : LayoutActor(pName, true), mParent(pParent), mPadChannel(padChannel), mAnimType(AnimType_NULL), mIsAppear(false), mAppearTime(0) {
    initLayoutManager("DPDPointerNumber", 1);
    startAnimPosition(0);
}

void StarPointerNumber::control() {
    if (MR::isConnectedWPad(WPAD_CHAN0) && MR::isConnectedWPad(WPAD_CHAN1)) {
        mIsAppear = true;
        mAppearTime++;
    } else {
        mIsAppear = false;
        mAppearTime = 0;
    }

    if (mAppearTime == ::hHideRumbleTime) {
        MR::tryRumblePadWeak(this, mPadChannel);
    }

    if (mAppearTime == 40) {
        MR::startCSSound("CS_CONT_CONNECT", "SE_SY_CS_CONT_CONNECT", mPadChannel);
    }

    if (!mIsAppear || isOffState()) {
        startAnimPosition(0);
        mAnimType = AnimType_NULL;
        return;
    }

    switch (mParent->mAnimType) {
    case AnimType_Finger:
        startAnimPosition(2);
        break;
    case AnimType_HandGuu:
        startAnimPosition(3);
        break;
    case AnimType_HandPaa:
        startAnimPosition(3);
        break;
    case AnimType_StarPointer:
        startAnimPosition(1);
        break;
    }

    mAnimType = mParent->mAnimType;
}

bool StarPointerNumber::isOffState() const {
    return mParent->isNerve(&NrvStarPointerLayout::HostTypeNrvOutScreen::sInstance) ||
           mParent->isNerve(&NrvStarPointerLayout::HostTypeNrvNotConnected::sInstance) ||
           mParent->isNerve(&NrvStarPointerLayout::HostTypeNrvHide::sInstance) ||
           mParent->isNerve(&NrvStarPointerLayout::HostTypeNrvHideImmediately::sInstance) ||
           mParent->isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidate::sInstance) ||
           mParent->isNerve(&NrvStarPointerLayout::HostTypeNrvInvalidateWait::sInstance);
}

void StarPointerNumber::startAnimPosition(u32 animType) {
    MR::startAnimAndSetFrameAndStop(this, "Position", animType + (mPadChannel == WPAD_CHAN0 ? 0 : 10), 0);
}
