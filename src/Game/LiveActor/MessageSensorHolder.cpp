#include "Game/LiveActor/MessageSensorHolder.hpp"
#include "Game/Util.hpp"

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