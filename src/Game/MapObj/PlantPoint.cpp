#include "Game/MapObj/PlantPoint.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/mtx.h>

PlantPoint::PlantPoint(const TVec3f& pPosition, const TVec3f& pUp, f32 thickness)
    // this constructor is patterned a bit weird... this only matches if mPosition
    // *specifically* is constructed as this, but this feels like a hack. Possibly
    // some TVec3 inline shenanigans to look into in the future.
    : mPosition(pPosition.x, pPosition.y, pPosition.z), mSide(1.0f, 0.0f, 0.0f), mUp(pUp), mFront(0.0f, 0.0f, 1.0f), mThickness(thickness) {
    TVec3f front;
    PSVECCrossProduct(&mSide, &mUp, &front);

    if (MR::isNearZero(front)) {
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
    } else {
        MR::makeAxisUpSide(&mFront, &mSide, mUp, mSide);
    }
}

void PlantPoint::setAxisAndPos(const TVec3f& pSide, const TVec3f& pUp, const TVec3f& pFront, const TVec3f& pPosition) {
    mSide.set(pSide);
    mUp.set(pUp);
    mFront.set(pFront);
    mPosition.set(pPosition);
}
