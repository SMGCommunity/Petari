#include "Game/Enemy/WalkerStateFunction.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

bool WalkerStateFunction::isInSightPlayer(const LiveActor* pActor, const TVec3f& pTVec3f, const WalkerStateParam* pParam) {
    return MR::isInSightFanPlayer(pActor, pTVec3f, pParam->_C, pParam->_10, pParam->_14);
}

void WalkerStateFunction::calcPassiveMovement(LiveActor* pActor, const WalkerStateParam* pParam) {
    MR::reboundVelocityFromEachCollision(pActor, -1.0f, -1.0f, 0.0f, 0.0f);
    if (!MR::isOnGround(pActor)) {
        MR::addVelocityToGravity(pActor, pParam->_0);
        MR::attenuateVelocity(pActor, pParam->_4);
    } else {
        MR::attenuateVelocity(pActor, pParam->_8);
    }
}
