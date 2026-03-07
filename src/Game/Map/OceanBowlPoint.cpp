#include "Game/Map/OceanBowlPoint.hpp"
#include "Game/Util/MathUtil.hpp"

const f32 sSinRate0 = 0.003f;
const f32 sSinRate1 = 0.003f;
const f32 sSinRate2 = 0.003f;
const f32 sHeight0 = 40.0f;
const f32 sHeight1 = 30.0f;

OceanBowlPoint::OceanBowlPoint(const TVec3f& rVec) : mVertexPosition(rVec), mPosition(rVec) {
    mAlpha = 255;
    mWaveScale = 1.0f;
}

void OceanBowlPoint::updatePos(f32 a, f32 b) {
    if (mWaveScale == 0.0f)
        return;
    f32 z = mPosition.z;
    f32 x = mPosition.x;
    f32 height = mWaveScale * calcHeightStatic(a, b, x, z);
    mVertexPosition.set(TVec3f(0.0f, 1.0f, 0.0f));
    mVertexPosition.mult(height);
    JMathInlineVEC::PSVECAdd2(&mVertexPosition, &mPosition, &mVertexPosition);
}

f32 OceanBowlPoint::calcHeightStatic(f32 a, f32 b, f32 c, f32 d) {
    return (MR::sin((a + (sSinRate1 * c)) + (sSinRate2 * d)) * sHeight1) + (MR::sin(b + (sSinRate0 * d)) * sHeight0);
}

void OceanBowlPoint::reset(const TVec3f& rVec, f32 f) {
    mPosition.set(rVec);
    mVertexPosition.set(mPosition);
    mWaveScale = f;
    f32 v = 255.0f;
    mAlpha = v * (f32)((f64)f);
}