#include "Game/Map/WaterPoint.hpp"
#include "Game/Util.hpp"

void WaterPoint_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sSinRate0 = 0.003f;
    static const f32 sSinRate1 = 0.0003f;
    static const f32 sSinRate2 = 0.0025f;
    static const f32 sMapCheckHeight = 400.0f;
    static const f32 sMapWaveRateMin = 0.1f;
    static const f32 sAlphaMin = 30.0f;
};  // namespace

WaterPoint::WaterPoint(const TVec3f& rPos, const TVec3f& rUpVec, f32 coordAcrossRail, f32 coordOnRail, f32 height, f32 flowSpeedRate)
    : mPosition(rPos), mOrigPos(rPos), mCoordAcrossRail(coordAcrossRail), mCoordOnRail(coordOnRail), mUpVec(rUpVec), mHeight(height),
      mFlowSpeedRate(flowSpeedRate), mAlpha(-1) {
}

void WaterPoint::initAfterPlacement() {
    if (mHeight != 0.0f) {
        TVec3f startCheckPos(mPosition.x, (::sMapCheckHeight/2.0f + mPosition.y), mPosition.z);
        TVec3f mapCheckPos(0.0f, 0.0f, 0.0f);

        if (MR::calcMapGround(startCheckPos, &mapCheckPos, ::sMapCheckHeight)) {
            f32 heightDiff = ((mapCheckPos.y - mPosition.y) / (::sMapCheckHeight / 2.0f));
            f32 waveRate = MR::clamp((::sMapWaveRateMin + MR::abs(heightDiff)), ::sMapWaveRateMin, 1.0f);
            mAlpha = MR::clamp((::sAlphaMin + (255.0f * -heightDiff)), ::sAlphaMin, 255.0f);
            mHeight = MR::min(waveRate, mHeight);
        }
    }
}

void WaterPoint::updatePos(f32 waveTheta1, f32 waveTheta2, f32 waveHeight1, f32 waveHeight2, f32 a5) {
    f32 height = calcHeight(waveTheta1, waveTheta2, waveHeight1, waveHeight2, mCoordAcrossRail, mCoordOnRail);
    // inlined TVec scale(f32, TVec3f)?
    f32 v9 = a5 * height;
    mPosition.x = mUpVec.x * v9;
    mPosition.y = mUpVec.y * v9;
    mPosition.z = mUpVec.z * v9;
    mPosition.add(mOrigPos);
}

f32 WaterPoint::calcHeight(f32 waveTheta1, f32 waveTheta2, f32 waveHeight1, f32 waveHeight2, f32 coordAcrossRail, f32 coordOnRail) const {
    return mHeight * (waveHeight1 * MR::sin(waveTheta1 + ::sSinRate0 * coordAcrossRail + ::sSinRate1 * coordOnRail) +
                                 waveHeight2 * MR::sin(waveTheta2 + (::sSinRate2 * coordOnRail)));
}
