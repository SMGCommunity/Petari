#include "Game/Enemy/TerritoryMover.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JMath/JMath.hpp"

TerritoryMover::TerritoryMover(f32 a1) {
    _0 = a1;
}

/* not even close but will look at it later */
void TerritoryMover::decideNextTargetPos(const LiveActor *pActor) {
    TVec3f v11;
    MR::getRandomVector(&v11, 1.0f);
    MR::normalizeOrZero(&v11);
    const TVec3f* gravPtr = &pActor->mGravity;
    f32 dot = gravPtr->dot(v11);
    JMAVECScaleAdd(gravPtr, &v11, &v11, -dot);
    f32 x = v11.x * _0;
    f32 y = v11.y * _0;
    f32 z = v11.z * _0;
    v11.x = x;
    v11.y = y;
    v11.z = z;

    JMathInlineVEC::PSVECAdd(&v11, &v11, &_4);
    JMathInlineVEC::PSVECAdd(&_4, &_4, &_10);
}

// TerritoryMover::isReachedTarget
