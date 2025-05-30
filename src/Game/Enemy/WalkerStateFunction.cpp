#include "Game/Enemy/WalkerStateFunction.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Util.hpp"

void WalkerStateFunction::calcPassiveMovement(LiveActor *pActor, const WalkerStateParam *pParam) {
    MR::reboundVelocityFromEachCollision(pActor, -1.0f, -1.0f, 0.0f, 0.0f);
    if (MR::isOnGround(pActor)) {
        MR::attenuateVelocity(pActor, pParam->_8);
    }
    else {
        MR::addVelocityToGravity(pActor, pParam->_0);
        MR::attenuateVelocity(pActor, pParam->_4);
    }
}
