#include "Game/Screen/StarPointerController.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>

namespace {
    // static const _32 hMarginH =
    // static const _32 hMarginV =
    // static const _32 hForcePointerOutVel =
    static const s32 hIsOutScreenCounter = 35;
    // static const _32 hNearValue =
    // static const _32 hDrawDebugRadius =
}  // namespace

DpdInfo::DpdInfo() : mPos(0.0f, 0.0f), mInScreen(false), _9(false), _C(0), mViewDistZ(0.0f) {
}

StarPointerController::StarPointerController()
    : mPadChannel(-1), mScreenPos(0.0f, 0.0f), mOutScreenTime(0), mPrevPastPointingPos(nullptr), mPrevPastCount(0), mPastPointingPos(nullptr),
      mPastCount(0), mScreenDist(0.0f), mScreenSpeed(0.0f), mScreenVel(0.0f, 0.0f), mWorldVel(0.0f, 0.0f, 0.0f), mWorldPos(0.0f, 0.0f, 0.0f) {
    // FIXME: TVec2 emitting default ctor in new[], needs to be a null ctor.
    mPrevPastPointingPos = new TVec2f[16];
    mPastPointingPos = new TVec2f[16];
}

bool StarPointerController::isOutScreenLong() const {
    return mOutScreenTime > hIsOutScreenCounter;
}

void StarPointerController::initAndSetPort(s32 padChannel) {
    mPadChannel = padChannel;
}

void StarPointerController::movement(const f32* f1, const f32* f2) {
    storeDataFromCallback(f1, f2);
    storePastPointingData();
    updateDpdInfo();
}

void StarPointerController::storeDataFromCallback(const f32* f1, const f32* f2) {
    TVec3f v1(mInfo.mPos.x, mInfo.mPos.y, static_cast< f32 >(mInfo._C));

    if (mInfo._9 == true) {
        TDDraw::invProject(&mWorldPos, v1, MR::getStarPointerViewMtx(), f1, f2, false);
        mInfo.mViewDistZ = calcViewDistanceZ(mWorldPos, MR::getStarPointerViewMtx());
    }

    mPastInfo = mInfo;
    if (mPastInfo.mInScreen) {
        mOutScreenTime = 0;
    } else {
        mOutScreenTime++;
    }

    mInfo._9 = false;
}

void StarPointerController::storePastPointingData() {
    mPrevPastCount = mPastCount;
    for (s32 idx = 0; idx < mPastCount; idx++) {
        mPrevPastPointingPos[idx] = mPastPointingPos[idx];
    }

    s32 pastCount = MR::min(MR::getCorePadEnablePastCount(mPadChannel), 16);
    mPastCount = pastCount;

    for (s32 idx = 0; idx < pastCount; idx++) {
        calcPastPointingPosOnScreen(&mPastPointingPos[idx], idx);
    }
}

void StarPointerController::calcPastPointingPosOnScreen(TVec2f* pPos, s32 idx) const {
    MR::getCorePadPastPointingPos(pPos, idx, mPadChannel);
    pPos->x += 1.0f;
    pPos->y += 1.0f;
    pPos->x *= MR::getScreenWidth() * 0.5f;
    pPos->y *= static_cast< s32 >(JUTVideo::getManager()->getEfbHeight()) * 0.5f;
}

void StarPointerController::updateDpdInfo() {
    if (!MR::isCorePadPointInScreen(mPadChannel)) {
        TVec2f v1;
        v1.x = mInfo.mPos.x;
        v1.y = mInfo.mPos.y;
        mInfo.mInScreen = false;
        mScreenPos.set(v1);
        updateAdditionalInfo();
    } else {
        mScreenPos.set(mInfo.mPos);
        mInfo.mInScreen = true;
        mInfo.mPos.set(mPastPointingPos[0]);
        mNotInScreen = StarPointerFunction::forceInsideScreenEdge(&mInfo.mPos);
        mScreenDist = MR::getCorePadDistanceToDisplay(mPadChannel);
        updateAdditionalInfo();
    }
}

void StarPointerController::updateAdditionalInfo() {
    mScreenVel.set(mInfo.mPos);
    mScreenVel.sub(mScreenPos);
    mScreenSpeed = mScreenVel.length();
    mWorldVel.set< f32 >(mScreenVel.x, -mScreenVel.y, 0.0f);
    TPos3f mtx;
    PSMTXCopy(MR::getStarPointerViewMtx(), mtx);
    PSMTXInverse(mtx, mtx);
    PSMTXMultVecSR(mtx, &mWorldVel, &mWorldVel);
    MR::normalizeOrZero(&mWorldVel);
}

f32 StarPointerController::calcViewDistanceZ(const TVec3f& rPos, MtxPtr pMtx) {
    return __fabsf(rPos.x * pMtx[2][0] + rPos.y * pMtx[2][1] + rPos.z * pMtx[2][2] + pMtx[2][3]);
}

// NOTE: this is a debug symbol, TVec2f::sub is included here just to emit TVec symbol
void StarPointerController::drawDebug3D() const {
    TVec2f v1 = mScreenVel;
    v1.sub(mScreenPos);
}
