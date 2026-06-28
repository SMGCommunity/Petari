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

    randVec.rejection(pActor->mGravity);
    randVec.mult(_0);
    randVec += mCenter;
    _10 = randVec;
}

bool TerritoryMover::isReachedTarget(const LiveActor* pActor, f32 range) {
    // FIXME: come back during rejection pass
    // https://decomp.me/scratch/z8igI

    TVec3f planar;
    planar.rejection(_10 - pActor->mPosition, pActor->mGravity);
    return planar.length() < range;
}
