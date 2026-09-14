#include "Game/Map/WaterPoint.hpp"
#include "Game/Util.hpp"

void WaterPoint_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sSinRate0 = 0.003f;
    static const f32 sSinRate1 = 0.0003f;
    static const f32 sSinRate2 = 0.0025f;
    //static const f32 sMapCheckHeight;
    static const f32 sMapWaveRateMin = 0.1f;
    static const f32 sAlphaMin = 30.0f;
};  // namespace

WaterPoint::WaterPoint(const TVec3f& rPos, const TVec3f& rUpVec, f32 coordAcrossRail, f32 coordOnRail, f32 height, f32 flowSpeedRate)
    : mPosition(rPos), mOrigPos(rPos), mCoordAcrossRail(coordAcrossRail), mCoordOnRail(coordOnRail), mUpVec(rUpVec), mHeight(height),
      mFlowSpeedRate(flowSpeedRate), mAlpha(-1) {
}

void WaterPoint::initAfterPlacement() {
    if (mHeight != 0.0f) {
        TVec3f v9(mPosition.x, (200.0f + mPosition.y), mPosition.z);
        TVec3f v8(0.0f, 0.0f, 0.0f);

        if (MR::calcMapGround(v9, &v8, 400.0f)) {
            f32 v2 = ((v8.y - mPosition.y) / 200.0f);
            f32 v3 = MR::clamp((::sMapWaveRateMin + MR::abs(v2)), ::sMapWaveRateMin, 1.0f);
            f32 v6 = MR::clamp((::sAlphaMin + (255.0f * -v2)), ::sAlphaMin, 255.0f);
            mAlpha = v6;
            mHeight = MR::min(v3, mHeight);
        }
    }
}

void WaterPoint::updatePos(f32 waveTheta1, f32 waveTheta2, f32 waveHeight1, f32 waveHeight2, f32 a5) {
    f32 height = calcHeight(waveTheta1, waveTheta2, waveHeight1, waveHeight2, mCoordAcrossRail, mCoordOnRail);
    f32 v9 = a5 * height;
    mPosition.x = mUpVec.x * v9;
    mPosition.y = mUpVec.y * (a5 * height);
    mPosition.z = mUpVec.z * v9;
    mPosition.add(mOrigPos);
}

f32 WaterPoint::calcHeight(f32 waveTheta1, f32 waveTheta2, f32 waveHeight1, f32 waveHeight2, f32 coordAcrossRail, f32 coordOnRail) const {
    return mHeight * (waveHeight1 * MR::sin(waveTheta1 + ::sSinRate0 * coordAcrossRail + ::sSinRate1 * coordOnRail) +
                                 waveHeight2 * MR::sin(waveTheta2 + (::sSinRate2 * coordOnRail)));
}
