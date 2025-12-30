#include "Game/Enemy/TerritoryMover.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

TerritoryMover::TerritoryMover(f32 a1) {
    _0 = a1;
}

void TerritoryMover::decideNextTargetPos(const LiveActor* pActor) {
    TVec3f randVec;
    MR::getRandomVector(&randVec, 1.0f);
    MR::normalizeOrZero(&randVec);

    const TVec3f* gravity = &pActor->mGravity;
    f32 dot = gravity->dot(randVec);
    JMAVECScaleAdd(gravity, &randVec, &randVec, -dot);

    randVec.mult(_0);

    JMathInlineVEC::PSVECAdd(&randVec, &_4, &randVec);
    _10.setPS(randVec);
}

bool TerritoryMover::isReachedTarget(const LiveActor* pActor, f32 a2) {
    TVec3f planar;
    TVec3f diff(_10);
    const TVec3f* gravity = &pActor->mGravity;

    JMathInlineVEC::PSVECSubtract(&diff, &pActor->mPosition, &diff);
    JMAVECScaleAdd(gravity, &diff, &planar, -gravity->dot(diff));
    return PSVECMag(&planar) < a2;
}
