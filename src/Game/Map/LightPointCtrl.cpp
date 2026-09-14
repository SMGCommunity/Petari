#include "Game/Map/LightPointCtrl.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/MathUtil.hpp"

LightPointCtrl::LightPointCtrl()
    : mStep(-1), mInterpolate(30), mCurrentActor(nullptr), mPreviousActor(nullptr), mCandidateActor(nullptr), mCurrentInfo(nullptr),
      mTargetInfo(nullptr), mPreviousInfo(nullptr) {
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
        mTargetInfo->mRefDistance = 15.0f;
        mTargetInfo->mDistAttnFn = GX_DA_STEEP;
        mInterpolate = duration >= 0 ? duration : 30;
    }
}

void LightPointCtrl::updatePointLight() {
    if (!mCurrentActor && mPreviousActor) {
        clearPointLight(mTargetInfo);
    }

    if (!mCurrentActor && mStep == -1) {
        clearPointLight(mCurrentInfo);
        return;
    }

    if (getStep() == -1) {
        *mCurrentInfo = *mTargetInfo;
        return;
    }

    f32 t = MR::getEaseInOutValue((f32)mStep / mInterpolate, 0.0f, 1.0f, 1.0f);
    blendPointLight(mCurrentInfo, *mPreviousInfo, *mTargetInfo, t);

    if (mStep < mInterpolate) {
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
    pInfo->mRefDistance = 15.0f;
    pInfo->mDistAttnFn = GX_DA_STEEP;
    mInterpolate = 30;
}

void LightPointCtrl::blendPointLight(PointLightInfo* pDst, const PointLightInfo& rStart, const PointLightInfo& rEnd, f32 t) {
    if (!mCurrentActor) {
        pDst->mPos = rStart.mPos;
    } else if (!mPreviousActor) {
        pDst->mPos = rEnd.mPos;
    } else {
        MR::blendVec(&pDst->mPos, rStart.mPos, rEnd.mPos, t);
    }

    f32 start = !mPreviousActor ? 0.95f : rStart.mRefBrightness;
    f32 end = !mCurrentActor ? 0.95f : rEnd.mRefBrightness;
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
    if (!mCandidateActor) {
        return true;
    }

    f32 dist = MR::calcDistanceToPlayer(mCandidateActor);
    return MR::calcDistanceToPlayer(pActor) < dist;
}
