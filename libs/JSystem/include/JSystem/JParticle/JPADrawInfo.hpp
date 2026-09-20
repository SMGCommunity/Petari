#pragma once

#include <revolution/mtx.h>

class JPADrawInfo {
public:
    JPADrawInfo(const Mtx pCameraMtx) {
        PSMTXCopy(pCameraMtx, mCamMtx);
        PSMTXIdentity(mPrjMtx);
    }

    JPADrawInfo(const Mtx pCameraMtx, f32 fovY, f32 aspect) {
        PSMTXCopy(pCameraMtx, mCamMtx);
        C_MTXLightPerspective(mPrjMtx, fovY, aspect, 0.5f, -0.5f, 0.5f, 0.5f);
    }

    JPADrawInfo(const Mtx pCameraMtx, f32 top, f32 bottom, f32 left, f32 right) {
        PSMTXCopy(pCameraMtx, mCamMtx);
        C_MTXLightOrtho(mPrjMtx, top, bottom, left, right, 0.5f, 0.5f, 0.5f, 0.5f);
    }

    /* 0x00 */ Mtx mCamMtx;
    /* 0x30 */ Mtx mPrjMtx;

    void getCamMtx(Mtx pDst) const {
        PSMTXCopy(mCamMtx, pDst);
    }

    void getPrjMtx(Mtx pDst) const {
        PSMTXCopy(mPrjMtx, pDst);
    }

    void setPrjMtx(const Mtx pSrc) {
        PSMTXCopy(pSrc, mPrjMtx);
    }
};
