#include "Game/MapObj/SimpleNormalMapObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

SimpleNormalMapObj::SimpleNormalMapObj(const char* pName) : NormalMapBase(pName) {
}
SimpleNormalMapObj::~SimpleNormalMapObj() {
}

void SimpleNormalMapObj::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    char v6[0x100];
    MR::getMapPartsObjectNameIfExistShapeID(v6, sizeof(v6), rIter);
    initModelManagerWithAnm(v6, nullptr, false);
    MR::connectToSceneNormalMapObj(this);
    MR::initLightCtrlNoDrawMapObj(this);
    initNormalMap();
    if (MR::isExistCollisionResource(this, v6)) {
        initHitSensor(1);
        HitSensor* sensor = MR::addBodyMessageSensorMapObj(this);
        MR::initCollisionParts(this, v6, sensor, nullptr);
    }
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    makeActorAppeared();
}
