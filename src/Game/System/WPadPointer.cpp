#include "Game/System/WPadPointer.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>

void WPadPointer_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)0.03f;
    (void)0.0001f;
}

namespace {
    static const f32 sMoveJudgeSpeed = 0.0001f;
    static const s32 sIsValidInScreenTime = 5;
    static const s32 sIsInValidOutScreenTime = 10;
    static const s32 sLostSensorTime = 20;
};  // namespace

WPadPointer::WPadPointer(const WPad* pPad) {
    mPad = pPad;
    mPosPlayRadius = 0.03f;
    mPosSensitivity = 0.5f;
    mHoriPlayRadius = 0.0f;
    mHoriSensitivity = 1.0f;
    mDistPlayRadius = 0.0f;
    mDistSensitivity = 1.0f;
    mLostSensorTime = 0;
    mDistDisplay = 0.0f;
    mDpdValidFg = 0;
    mValidInScreenTime = 0;
    mInValidOutScreenTime = 0;
    mEnablePastCount = 0;
    mIsPointInScreen = false;
    mIsPointerMoved = false;
    mPointingPosArray = new TVec2f[120];
    mHorizonArray = new TVec2f[120];
    mMaxPastCount = 120;
    reset();
}

void WPadPointer::reset() {
    for (s32 i = 0; i < mMaxPastCount; i++) {
        TVec2f* cur = &mPointingPosArray[i];
        cur->x = 0.0f;
        cur->y = 0.0f;
        cur = &mHorizonArray[i];
        cur->x = 0.0f;
        cur->y = 0.0f;
    }

    mDpdValidFg = 0;
    mDistDisplay = 0.0f;
    mValidInScreenTime = 0;
    mEnablePastCount = 0;
    mLostSensorTime = 0;
    mIsPointInScreen = false;
    mIsPointerMoved = false;
    KPADSetPosParam(mPad->mChannel, mPosPlayRadius, mPosSensitivity);
    KPADSetHoriParam(mPad->mChannel, mHoriPlayRadius, mHoriSensitivity);
    KPADSetDistParam(mPad->mChannel, mDistPlayRadius, mDistSensitivity);
}

void WPadPointer::setSensorBarLevel(f32 level) {
    KPADSetSensorHeight(mPad->mChannel, level);
}

void WPadPointer::update() {
    KPADStatus* status = mPad->getKPadStatus(0);

    if (status == nullptr) {
        reset();
        return;
    }

    mIsPointerMoved = false;
    mDistDisplay = status->dist;
    mDpdValidFg = status->dpd_valid_fg;

    s32 validCount = mPad->getValidStatusCount();
    mEnablePastCount = 0;

    if (validCount > mMaxPastCount) {
        validCount = mMaxPastCount;
    }

    bool isAnyDpdValid = false;

    for (s32 i = validCount - 1; i >= 0; i--) {
        KPADStatus* curStatus = mPad->getKPadStatus(i);
        s8 dpdValidFg = curStatus->dpd_valid_fg;

        if (dpdValidFg > 0) {
            isAnyDpdValid = true;
        }

        if (dpdValidFg < 2) {
            mValidInScreenTime = 0;
            mInValidOutScreenTime++;
        } else {
            if (!mIsPointInScreen && mValidInScreenTime >= ::sIsValidInScreenTime ||
                mIsPointInScreen && mInValidOutScreenTime <= ::sIsInValidOutScreenTime) {
                mPointingPosArray[mEnablePastCount].x = curStatus->pos.x;
                mPointingPosArray[mEnablePastCount].y = curStatus->pos.y;
                mHorizonArray[mEnablePastCount].x = curStatus->horizon.x;
                mHorizonArray[mEnablePastCount].y = curStatus->horizon.y;

                if (curStatus->speed > ::sMoveJudgeSpeed) {
                    mIsPointerMoved = true;
                }

                mEnablePastCount++;
            }

            mInValidOutScreenTime = 0;
            mValidInScreenTime++;
        }
    }

    mIsPointInScreen = mEnablePastCount > 0;

    if (isAnyDpdValid) {
        mLostSensorTime = 0;
    } else if (mLostSensorTime < ::sLostSensorTime) {
        mLostSensorTime++;
    }
}

void WPadPointer::getPointingPos(TVec2f* pOut) const {
    if (mIsPointInScreen) {
        pOut->set(mPointingPosArray[mEnablePastCount - 1]);
    } else {
        pOut->zero();
    }
}

void WPadPointer::getHorizonVec(TVec2f* pOut) const {
    if (mIsPointInScreen) {
        pOut->set(mHorizonArray[mEnablePastCount - 1]);
    } else {
        pOut->zero();
    }
}

void WPadPointer::getPastPointingPos(TVec2f* pOut, s32 idx) const {
    pOut->set(mPointingPosArray[mEnablePastCount - 1 - idx]);
}

s32 WPadPointer::getEnablePastCount() const {
    return mEnablePastCount;
}

void WPadPointer::getPointingPosBasedOnScreen(TVec2f* pOut) const {
    pOut->x = (0.5f + (0.5f * mPointingPosArray->x)) * static_cast< s32 >(MR::getScreenWidth());
    pOut->y = (0.5f + (0.5f * mPointingPosArray->y)) * static_cast< s32 >(JUTVideo::getManager()->getRenderMode()->efbHeight);
}
