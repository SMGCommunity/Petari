#include "Game/System/WPadPointer.hpp"
#include "Game/Util.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>

namespace {
    static const f32 sMoveJudgeSpeed = 0.0001f;
    static const s32 sIsValidInScreenTime = 5;
    static const s32 sIsInValidOutScreenTime = 10;
    static const s32 sLostSensorTime = 20;
};  // namespace

// arrays are generating a constructor for some reason
WPadPointer::WPadPointer(const WPad* pPad) {
    mPad = pPad;
    mPosPlayRadius = 0.03f;
    mPosSensitivity = 0.5f;
    mHoriPlayRadius = 0.0;
    mHoriSensitivity = 1.0f;
    mDistPlayRadius = 0.0f;
    mDistSensitivity = 1.0f;
    _2C = 0;
    mDistDisplay = 0.0f;
    _34 = 0;
    _38 = 0;
    _3C = 0;
    mEnablePastCount = 0;
    mIsPointInScreen = false;
    mIsPointerMoved = false;
    mPointingPosArray = new TVec2f[0x78];
    mHorizonArray = new TVec2f[0x78];
    _C = 120;
    reset();
}

void WPadPointer::reset() {
    for (s32 i = 0; i < _C; i++) {
        TVec2f* cur = &mPointingPosArray[i];
        cur->x = 0.0f;
        cur->y = 0.0f;
        cur = &mHorizonArray[i];
        cur->x = 0.0f;
        cur->y = 0.0f;
    }

    _34 = 0;
    mDistDisplay = 0.0f;
    _38 = 0;
    mEnablePastCount = 0;
    _2C = 0;
    mIsPointInScreen = false;
    mIsPointerMoved = false;
    KPADSetPosParam(mPad->mChannel, mPosPlayRadius, mPosSensitivity);
    KPADSetHoriParam(mPad->mChannel, mHoriPlayRadius, mHoriSensitivity);
    KPADSetDistParam(mPad->mChannel, mDistPlayRadius, mDistSensitivity);
}

void WPadPointer::setSensorBarLevel(f32 lvl) {
    KPADSetSensorHeight(mPad->mChannel, lvl);
}

void WPadPointer::update() {
    KPADStatus* status = mPad->getKPadStatus(0);

    if (status == nullptr) {
        reset();
        return;
    }

    mIsPointerMoved = false;
    mDistDisplay = status->dist;
    _34 = status->dpd_valid_fg;

    s32 validCount = mPad->getValidStatusCount();
    mEnablePastCount = 0;

    if (validCount > _C) {
        validCount = _C;
    }

    bool isAnyDPDValid = false;

    for (s32 i = validCount - 1; i >= 0; i--) {
        KPADStatus* curStatus = mPad->getKPadStatus(i);
        s8 dpdValid = curStatus->dpd_valid_fg;

        if (dpdValid > 0) {
            isAnyDPDValid = true;
        }

        if (dpdValid < 2) {
            _38 = 0;
            _3C++;
        } else {
            if (!mIsPointInScreen && _38 >= ::sIsValidInScreenTime || mIsPointInScreen && _3C <= ::sIsInValidOutScreenTime) {
                mPointingPosArray[mEnablePastCount].x = curStatus->pos.x;
                mPointingPosArray[mEnablePastCount].y = curStatus->pos.y;
                mHorizonArray[mEnablePastCount].x = curStatus->horizon.x;
                mHorizonArray[mEnablePastCount].y = curStatus->horizon.y;

                if (curStatus->speed > ::sMoveJudgeSpeed) {
                    mIsPointerMoved = true;
                }

                mEnablePastCount++;
            }

            _3C = 0;
            _38++;
        }
    }

    mIsPointInScreen = mEnablePastCount > 0;

    if (isAnyDPDValid) {
        _2C = 0;
    } else if (_2C < ::sLostSensorTime) {
        _2C++;
    }
}

void WPadPointer::getPointingPos(TVec2f* pOut) const {
    if (mIsPointInScreen) {
        pOut->set(mPointingPosArray[mEnablePastCount - 1]);
    } else {
        pOut->y = 0.0f;
        pOut->x = 0.0f;
    }
}

void WPadPointer::getHorizonVec(TVec2f* pOut) const {
    if (mIsPointInScreen) {
        pOut->set(mHorizonArray[mEnablePastCount - 1]);
    } else {
        pOut->y = 0.0f;
        pOut->x = 0.0f;
    }
}

void WPadPointer::getPastPointingPos(TVec2f* pOut, s32 idx) const {
    pOut->set(mPointingPosArray[mEnablePastCount - 1 - idx]);
}

u32 WPadPointer::getEnablePastCount() const {
    return mEnablePastCount;
}

void WPadPointer::getPointingPosBasedOnScreen(TVec2f* pOut) const {
    pOut->x = (0.5f + (0.5f * mPointingPosArray->x)) * static_cast< s32 >(MR::getScreenWidth());
    pOut->y = (0.5f + (0.5f * mPointingPosArray->y)) * static_cast< s32 >(JUTVideo::getManager()->getRenderMode()->efbHeight);
}
