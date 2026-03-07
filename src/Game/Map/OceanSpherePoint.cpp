#include "Game/Map/OceanSpherePoint.hpp"
#include "Game/Util.hpp"
#include "revolution/mtx.h"

OceanSpherePoint::OceanSpherePoint(const TVec3f* pCenter, const TVec3f& rNormal, f32 wave1Pos, f32 wave2Pos, const TVec2f& rTexCoord)
    : mPos(*pCenter), mCenter(pCenter), mNormal(rNormal), mWave1Pos(wave1Pos), mWave2Pos(wave2Pos), mTexCoord(rTexCoord) {}

void OceanSpherePoint::updatePos(f32 radius, f32 wave1Time, f32 wave2Time) {
    f32 height = radius + calcHeight(wave1Time, wave2Time, mWave1Pos, mWave2Pos);
    mPos.x = mNormal.x * height;
    mPos.y = mNormal.y * height;
    mPos.z = mNormal.z * height;
    JMathInlineVEC::PSVECAdd(&mPos, mCenter, &mPos);
}

f32 OceanSpherePoint::calcHeight(f32 wave1Time, f32 wave2Time, f32 wave1Pos, f32 wave2Pos) const {
    f32 wave2 = MR::sin(wave2Time + wave2Pos) * 5.0f;
    return (10.0f * MR::sin(wave1Time + (0.8f * wave1Pos))) + wave2;
}
