#include "Game/Enemy/Balloonfish.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/JMapInfo.hpp"

namespace {
    // static const ??? hWaitTime = ???;
    // static const ??? hAttackBeginTime = ???;
    // static const ??? hTargetPlayerVelocityMult = ???;
    // static const ??? hWaitMaxScale = ???;
    // static const ??? hDashTime = ???;
    // static const ??? hDashVel = ???;
    // static const ??? hDashScale = ???;
    // static const ??? hDashEndTime = ???;
    // static const ??? hRotateAngle = ???;
}

namespace NrvBalloonFish {
    NEW_NERVE(HostTypeNrvStarPointerBind, Balloonfish, StarPointerBind);
    NEW_NERVE(HostTypeNrvDashEnd, Balloonfish, DashEnd);
    NEW_NERVE(HostTypeNrvDash, Balloonfish, Dash);
    NEW_NERVE(HostTypeNrvWait, Balloonfish, Wait);
};  // namespace NrvBalloonFish

Balloonfish::Balloonfish(const char* pName) : LiveActor(pName), _8C(), _90(), _98(0.0f, 0.0f, 0.0f, 1.0f), _A8() {
}
Balloonfish::~Balloonfish() {
}

void Balloonfish::init(const JMapInfoIter& rIter) {
}
