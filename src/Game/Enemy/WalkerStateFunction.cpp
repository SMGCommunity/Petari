#include "Game/Enemy/WalkerStateFunction.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

bool WalkerStateFunction::isInSightPlayer(const LiveActor* pActor, const TVec3f& rDirection, const WalkerStateParam* pStateParam) {
    return MR::isInSightFanPlayer(pActor, rDirection, pStateParam->mPlayerNearDistance, pStateParam->mPlayerSightFanDegreeH,
                                  pStateParam->mPlayerSightFanDegreeV);
}

void WalkerStateFunction::calcPassiveMovement(LiveActor* pActor, const WalkerStateParam* pStateParam) {
    MR::reboundVelocityFromEachCollision(pActor, -1.0f, -1.0f, 0.0f, 0.0f);
    if (!MR::isOnGround(pActor)) {
        MR::addVelocityToGravity(pActor, pStateParam->mGravityAccel);
        MR::attenuateVelocity(pActor, pStateParam->mAirFriction);
    } else {
        MR::attenuateVelocity(pActor, pStateParam->mGroundFriction);
    }
}
