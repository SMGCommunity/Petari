#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"

void MarioActor::bodyClap() {
    for (u32 i = 0; i < _7DC; i++) {
        TVec3f stack_14(_6DC[i]->mPosition);
        TVec3f stack_8(stack_14 - _2A0);
        if (stack_8.length() < 100.0f) {
            _6DC[i]->receiveMessage(ACTMES_ITEM_GET, getSensor("body"));
        }
    }
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
