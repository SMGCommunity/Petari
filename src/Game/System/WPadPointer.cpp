#include "Game/System/WPadPointer.hpp"
#include "Game/Util.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>

#ifdef NON_MATCHING
// arrays are generating a constructor for some reason
WPadPointer::WPadPointer(const WPad* pPad) {
    mPad = pPad;
    mPosPlayRadius = 0.029999999f;
    mPosSensitivity = 0.5f;
    mHoriPlayRadius = 0.0;
    mHoriSensitivity = 1.0f;
    mDistPlayRadius = 0.0f;
    mDistSensitivity = 1.0f;
    _2C = 0;
    _30 = 0.0f;
    _34 = 0;
    _38 = 0;
    _3C = 0;
    mEnablePastCount = 0;
    _44 = 0; 
    _45 = 0;
    mPointingPosArray = new TVec2f[0x78];
    mHorizonArray = new TVec2f[0x78];
    _C = 120;
    reset();
}
#endif

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
    _30 = 0.0f;
    _38 = 0;
    mEnablePastCount = 0;
    _2C = 0;
    _44 = 0; 
    _45 = 0;
    KPADSetPosParam(mPad->mChannel, mPosPlayRadius, mPosSensitivity);
    KPADSetHoriParam(mPad->mChannel, mHoriPlayRadius, mHoriSensitivity);
    KPADSetDistParam(mPad->mChannel, mDistPlayRadius, mDistSensitivity);
}


void WPadPointer::setSensorBarLevel(f32 lvl) {
    KPADSetSensorHeight(mPad->mChannel, lvl);
}

/*void WPadPointer::update() {
    KPADStatus* status = mPad->getKPadStatus(0);

    if (status != nullptr) {
        _45 = 0;
        _30 = status->dist;
        _34 = status->dpd_valid_fg;
        s32 validCount = mPad->getValidStatusCount();
        mEnablePastCount = 0;

        if (_C > validCount) {
            validCount = _C;
        }

        bool isAnyDPDValid = false;

        while (validCount - 1 >= 0) {
            KPADStatus* curStatus = mPad->getKPadStatus(validCount);

            if (curStatus->dpd_valid_fg) {
                isAnyDPDValid = true;
            }

            if (curStatus->dpd_valid_fg >= 2) {
                if (!_44 && _38 >= 5 || !_44 && _3C <= 10) {
                    mPointingPosArray[mEnablePastCount] = (TVec2f)curStatus->pos;
                }
            }
        }
    }
}*/


void WPadPointer::getPointingPos(TVec2f* pOut) const {
    if (_44 != 0) {
        pOut->set(mPointingPosArray[mEnablePastCount - 1]);
    }
    else {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
    }
}

void WPadPointer::getHorizonVec(TVec2f* pOut) const {
    if (_44 != 0) {
        pOut->set(mHorizonArray[mEnablePastCount - 1]);
    }
    else {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
    }
}

void WPadPointer::getPastPointingPos(TVec2f* pOut, s32 idx) const {
    pOut->set(mPointingPosArray[mEnablePastCount - 1 - idx]);
}

u32 WPadPointer::getEnablePastCount() const {
    return mEnablePastCount;
}

void WPadPointer::getPointingPosBasedOnScreen(TVec2f* pOut) const {
    pOut->x = (0.5f + (0.5f * mPointingPosArray->x)) * (int)MR::getScreenWidth();
    pOut->y = (0.5f + (0.5f * mPointingPosArray->y)) * (int)(JUTVideo::sManager->mRenderModeObj->efbHeight);
}