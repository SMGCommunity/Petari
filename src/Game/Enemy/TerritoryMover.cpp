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

    randVec.x *= _0;
    randVec.y *= _0;
    randVec.z *= _0;

    randVec.x += _4.x;
    randVec.y += _4.y;
    randVec.z += _4.z;

    _10.x = randVec.x;
    _10.y = randVec.y;
    _10.z = randVec.z;
}

bool TerritoryMover::isReachedTarget(const LiveActor* pActor, f32 a2) {
    TVec3f diff;
    diff.x = _10.x - pActor->mPosition.x;
    diff.y = _10.y - pActor->mPosition.y;
    diff.z = _10.z - pActor->mPosition.z;

    TVec3f planar;
    JMAVECScaleAdd(&pActor->mGravity, &diff, &planar, -pActor->mGravity.dot(diff));
    return PSVECMag(&planar) < a2;
}
