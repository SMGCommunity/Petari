#include "Game/MapObj/SimpleNormalMapObj.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

SimpleNormalMapObj::SimpleNormalMapObj(const char* pName) : NormalMapBase(pName) {
}

void SimpleNormalMapObj::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    char objectName[256];
    MR::getMapPartsObjectNameIfExistShapeID(objectName, sizeof(objectName), rIter);

    initModelManagerWithAnm(objectName, nullptr, false);
    MR::connectToSceneNormalMapObj(this);
    MR::initLightCtrlNoDrawMapObj(this);
    initNormalMap();

    if (MR::isExistCollisionResource(this, objectName)) {
        initHitSensor(1);
        MR::initCollisionParts(this, objectName, MR::addBodyMessageSensorMapObj(this), nullptr);
    }

    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    makeActorAppeared();
}
