#include "Game/LiveActor/MessageSensorHolder.hpp"
#include "Game/Util.hpp"

MessageSensorHolder::MessageSensorHolder(const char *pName) :
    LiveActor(pName)
{
    
}

void MessageSensorHolder::init(const JMapInfoIter &rIter) {
    initHitSensor(1);
    MR::addBodyMessageSensor(this, ATYPE_MESSAGE_SENSOR);
    MR::invalidateClipping(this);
    makeActorAppeared();
}
