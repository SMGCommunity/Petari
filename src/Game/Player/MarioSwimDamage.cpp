#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioSwim.hpp"

bool Mario::checkWaterDamage() {
    if (mMovementStates._1B) {
        mMovementStates._1B = 0;
        mSwim->addDamage(_7C4);
        return true;
    } else if (mMovementStates._27) {
        mMovementStates._27 = 0;
        mSwim->addFaint(_7C4);
        return true;
    } else {
        return false;
    }
}
