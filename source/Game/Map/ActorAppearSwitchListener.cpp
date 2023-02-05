#include "Game/Map/ActorAppearSwitchListener.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Util.h"

ActorAppearSwitchListener::ActorAppearSwitchListener(LiveActor *pActor, bool usesOn, bool usesOff) : SwitchEventListener() {
    mActor = pActor;
    mUsesOn = usesOn;
    mUsesOff = usesOff;
}

void ActorAppearSwitchListener::listenSwitchOnEvent() {
    if (MR::isDead(mActor) && mUsesOn) {
        mActor->appear();
    }
}

void ActorAppearSwitchListener::listenSwitchOffEvent() {
    if (!MR::isDead(mActor) && mUsesOff) {
        mActor->kill();
    }
}