#include "Game/MapObj/NormalMapTestObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/NormalMapBase.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

NormalMapTestObj::NormalMapTestObj(const char* pName) : NormalMapBase(pName) {
}
NormalMapTestObj::~NormalMapTestObj() {
}

void NormalMapTestObj::init(const JMapInfoIter& rrIter) {
    MR::initDefaultPos(this, rrIter);
    NormalMapBase::setup("NormalMapTestObj");
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::invalidateClipping(this);
    appear();
}
