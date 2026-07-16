#include "Game/Enemy/TerritoryMover.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

TerritoryMover::TerritoryMover(f32 radius) {
    mRadius = radius;
}

void TerritoryMover::decideNextTargetPos(const LiveActor* pActor) {
    TVec3f randVec;
    MR::getRandomVector(&randVec, 1.0f);
    MR::normalizeOrZero(&randVec);

    randVec.orthogonalize(pActor->mGravity);
    randVec *= mRadius;
    randVec += mCenter;
    mTarget = randVec;
}

bool TerritoryMover::isReachedTarget(const LiveActor* pActor, f32 range) {
    TVec3f planar;
    TVec3f diff = mTarget - pActor->mPosition;
    planar.killElement(diff, pActor->mGravity);
    return planar.length() < range;
}
