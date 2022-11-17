#include "Game/Map/WaterPoint.h"
#include "Game/Util.h"

WaterPoint::WaterPoint(const TVec3f &rPos, const TVec3f &rUpVec, f32 coordAcrossRail, f32 coordOnRail, f32 height, f32 flowSpeedRate) :
    mPosition(rPos), mOrigPos(rPos), mCoordAcrossRail(coordAcrossRail), mCoordOnRail(coordOnRail), mUpVec(rUpVec), mHeight(height), mFlowSpeedRate(flowSpeedRate), mAlpha(-1) {

}

void WaterPoint::initAfterPlacement() {
    if (mHeight != 0.0f) {
        TVec3f v9(mPosition.x, (200.0f + mPosition.y), mPosition.z);
        TVec3f v8(0.0f, 0.0f, 0.0f);

        if (MR::calcMapGround(v9, &v8, 400.0f)) {
            f32 v2 = ((v8.y - mPosition.y) / 200.0f);
            f32 v3 = MR::clamp((0.1f + __fabsf(v2)), 0.1f, 1.0f);
            f32 v6 = MR::clamp((30.0f + (255.0f * -v2)), 30.0f, 255.0f);
            f32 v7 = mHeight;
            mAlpha = v6;
            mHeight = (v3 >= v7) ? v7 : v3;
        }
    }
}

void WaterPoint::updatePos(f32 waveTheta1, f32 waveTheta2, f32 waveHeight1, f32 waveHeight2, f32 a5) {
    f32 height = calcHeight(waveTheta1, waveTheta2, waveHeight1, waveHeight2, mCoordAcrossRail, mCoordOnRail);
    f32 v9 = a5 * height;
    mPosition.x = mUpVec.x * v9;
    mPosition.y = mUpVec.y * (a5 * height); 
    mPosition.z = mUpVec.z * v9;
    mPosition.addInline_4(mOrigPos);
}

/* thanks to WMC for naming the vars */
f32 WaterPoint::calcHeight(f32 waveTheta1, f32 waveTheta2, f32 waveHeight1, f32 waveHeight2, f32 coordAcrossRail, f32 coordOnRail) const {
    // "Wave 2" moves along the rail
    f32 actualWaveHeight2 = (waveHeight2 * MR::sin(waveTheta2 + (0.0024999999f * coordOnRail)));

    // "Wave 1" moves diagonal to the rail
    f32 wave1PerpendicularComponent = 0.003f * coordAcrossRail;
    f32 wave1ThetaPlusPerpendicularComponent = waveTheta1 + wave1PerpendicularComponent;
    f32 normalizedWaveHeight1 = MR::sin(wave1ThetaPlusPerpendicularComponent + 0.00030000001f * coordOnRail);
    f32 actualWaveHeight1 = waveHeight1 * normalizedWaveHeight1;

    f32 finalHeight = mHeight * (actualWaveHeight2 + actualWaveHeight1);
    return finalHeight;
}