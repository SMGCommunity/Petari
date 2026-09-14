#include "Game/Map/LightPointCtrl.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace {
    static const s32 sDefaultBlendTime = 30;
    static const f32 sDefaultDistRef = 15.0f;
};

LightPointCtrl::LightPointCtrl()
    : mStep(-1), mBlendTime(::sDefaultBlendTime), mCurrentActor(), mPreviousActor(), mCandidateActor(), mCurrentInfo(),
      mTargetInfo(), mPreviousInfo() {
    mCurrentInfo = new PointLightInfo();
    mTargetInfo = new PointLightInfo();
    mPreviousInfo = new PointLightInfo();

    clearPointLight(mCurrentInfo);
    clearPointLight(mTargetInfo);
    clearPointLight(mPreviousInfo);
}

void LightPointCtrl::loadPointLight() {
    LightFunction::loadPointLightInfo(mCurrentInfo);
}

void LightPointCtrl::update() {
    if (mStep == -1) {
        mCurrentActor = mCandidateActor;
        tryBlendStart();
    }

    updatePointLight();

    if (mStep == -1) {
        mPreviousActor = mCurrentActor;
        *mPreviousInfo = *mTargetInfo;
        mCandidateActor = nullptr;
    }
}

void LightPointCtrl::requestPointLight(const LiveActor* pActor, TVec3f pos, Color8 color, f32 intensity, s32 duration) {
    if (mStep == -1 && isUpdateCandidateActor(pActor)) {
        mCandidateActor = pActor;
        mTargetInfo->mPos = pos;
        mTargetInfo->mColor = color.mGXColor;
        mTargetInfo->mRefBrightness = MR::clamp(intensity, 0.95f, 0.999999f);
        mTargetInfo->mRefDistance = ::sDefaultDistRef;
        mTargetInfo->mDistAttnFn = GX_DA_STEEP;
        mBlendTime = duration >= 0 ? duration : ::sDefaultBlendTime;
    }
}

void LightPointCtrl::updatePointLight() {
    if (mCurrentActor == nullptr && mPreviousActor != nullptr) {
        clearPointLight(mTargetInfo);
    }

    if (mCurrentActor == nullptr && mStep == -1) {
        clearPointLight(mCurrentInfo);
        return;
    }

    if (getStep() == -1) {
        *mCurrentInfo = *mTargetInfo;
        return;
    }

    f32 t = MR::getEaseInOutValue(static_cast< f32 >(mStep) / mBlendTime, 0.0f, 1.0f, 1.0f);
    blendPointLight(mCurrentInfo, *mPreviousInfo, *mTargetInfo, t);

    if (mStep < mBlendTime) {
        mStep++;
    } else {
        mStep = -1;
    }
}

void LightPointCtrl::clearPointLight(PointLightInfo* pInfo) {
    Vec pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    pInfo->mPos = pos;

    GXColor color;
    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = 255;
    pInfo->mColor = color;

    pInfo->mRefBrightness = 0.001f;
    pInfo->mRefDistance = ::sDefaultDistRef;
    pInfo->mDistAttnFn = GX_DA_STEEP;
    mBlendTime = ::sDefaultBlendTime;
}

void LightPointCtrl::blendPointLight(PointLightInfo* pDst, const PointLightInfo& rStart, const PointLightInfo& rEnd, f32 t) {
    if (mCurrentActor == nullptr) {
        pDst->mPos = rStart.mPos;
    } else if (mPreviousActor == nullptr) {
        pDst->mPos = rEnd.mPos;
    } else {
        MR::blendVec(&pDst->mPos, rStart.mPos, rEnd.mPos, t);
    }

    f32 start = mPreviousActor == nullptr ? 0.95f : rStart.mRefBrightness;
    f32 end = mCurrentActor == nullptr ? 0.95f : rEnd.mRefBrightness;
    pDst->mRefBrightness = MR::getLinerValue(t, start, end, 1.0f);
    MR::blendColor(&pDst->mColor, rStart.mColor, rEnd.mColor, t);
    pDst->mRefDistance = MR::getLinerValue(t, rStart.mRefDistance, rEnd.mRefDistance, 1.0f);
}

bool LightPointCtrl::tryBlendStart() {
    if (mPreviousActor == mCurrentActor) {
        return false;
    }

    mStep = 0;
    return true;
}

bool LightPointCtrl::isUpdateCandidateActor(const LiveActor* pActor) const {
    if (mCandidateActor == nullptr) {
        return true;
    }

    f32 dist = MR::calcDistanceToPlayer(mCandidateActor);
    return MR::calcDistanceToPlayer(pActor) < dist;
}
