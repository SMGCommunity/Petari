#include "Game/MapObj/MercatorFixParts.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

MercatorFixParts::MercatorFixParts(const char* pName) : LiveActor(pName) {
    mAppearController = nullptr;
}

bool MercatorFixParts::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return mAppearController->receiveMsg(msg);
}

MercatorFixParts::~MercatorFixParts() {
}

void MercatorFixParts::init(const JMapInfoIter& rIter) {
    char objectName[256];
    MR::getMapPartsObjectName(objectName, sizeof(objectName), rIter);
    MR::initDefaultPosForMercator(this, rIter, false);
    initModelManagerWithAnm(objectName, nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 4, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, objectName, getSensor("body"), nullptr);
    mAppearController = new MapPartsAppearController(this);
    mAppearController->init(rIter);
    mAppearController->start();

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }

    MR::initMapPartsClipping(this, rIter, nullptr, false);
}