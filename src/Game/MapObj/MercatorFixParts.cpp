#include "Game/MapObj/MercatorFixParts.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"

MercatorFixParts::MercatorFixParts(const char *pName) : LiveActor(pName) {
    mAppearController = nullptr;
}

bool MercatorFixParts::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    return mAppearController->receiveMsg(msg);
}

MercatorFixParts::~MercatorFixParts() {
 
}

void MercatorFixParts::init(const JMapInfoIter &rIter) {
    char name[0x100];
    MR::getMapPartsObjectName(name, sizeof(name), rIter);
    MR::initDefaultPosForMercator(this, rIter, false);
    initModelManagerWithAnm(name, nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 4, 0.0f, offs);
    MR::initCollisionParts(this, name, getSensor("body"), nullptr);
    mAppearController = new MapPartsAppearController(this);
    mAppearController->init(rIter);
    mAppearController->start();

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }

    MR::initMapPartsClipping(this, rIter, nullptr, false);
}