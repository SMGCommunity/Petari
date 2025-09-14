#include "Game/Enemy/WalkerStateFunction.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

//Required For Float Order, should get stripped when linked.
void unusedFunction() {
    f32 x = 0.0f;
}

void WalkerStateFunction::calcPassiveMovement(LiveActor* pActor, const WalkerStateParam* pParam) {
    MR::reboundVelocityFromEachCollision(pActor, -1.0f, -1.0f, 0.0f, 0.0f);
    if (!MR::isOnGround(pActor)) {
        MR::addVelocityToGravity(pActor, pParam->_0);
        MR::attenuateVelocity(pActor, pParam->_4);
    }
    else {
        MR::attenuateVelocity(pActor, pParam->_8);
    }
}

bool WalkerStateFunction::isInSightPlayer(const LiveActor* pActor, const TVec3f& pTVec3f, const WalkerStateParam* pParam) {
    return MR::isInSightFanPlayer(pActor, pTVec3f, pParam->_C, pParam->_10, pParam->_14);
}
