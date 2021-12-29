#include "Game/LiveActor/MessageSensorHolder.h"
#include "Game/Util.h"

MessageSensorHolder::MessageSensorHolder(const char *pName) : LiveActor(pName) {

}

void MessageSensorHolder::init(const JMapInfoIter &rIter) {
    initHitSensor(1);
    MR::addBodyMessageSensor(this, 0x83);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

MessageSensorHolder::~MessageSensorHolder() {
    
}