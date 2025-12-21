#include "Game/MapObj/PlantPoint.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/mtx.h>

PlantPoint::PlantPoint(const TVec3f& pPosition, const TVec3f& pUp, f32 thickness)
    // this constructor is patterned a bit weird... this only matches if mPosition
    // *specifically* is constructed as this, but this feels like a hack. Possibly
    // some TVec3 inline shenanigans to look into in the future.
    : mPosition(pPosition.x, pPosition.y, pPosition.z), mFront(1.0f, 0.0f, 0.0f), mUp(pUp), mSide(0.0f, 0.0f, 1.0f), mThickness(thickness) {
    TVec3f side;
    PSVECCrossProduct(&mFront, &mUp, &side);

    if (MR::isNearZero(side)) {
        MR::makeAxisUpFront(&mFront, &mSide, mUp, mSide);
    } else {
        MR::makeAxisUpSide(&mSide, &mFront, mUp, mFront);
    }
}

void PlantPoint::setAxisAndPos(const TVec3f& pFront, const TVec3f& pUp, const TVec3f& pSide, const TVec3f& pPosition) {
    mFront.set(pFront);
    mUp.set(pUp);
    mSide.set(pSide);
    mPosition.set(pPosition);
}
