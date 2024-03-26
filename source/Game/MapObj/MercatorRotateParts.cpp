#include "Game/MapObj/MercatorRotateParts.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"

MercatorRotateParts::MercatorRotateParts(const char *pName) : LiveActor(pName) {
    mRotator = nullptr;
    mAppearController = nullptr;
}

void MercatorRotateParts::initAfterPlacement() {
    TVec3f trans;
    TPos3f rotate;
    MR::convertMercatorPlaneToSphereTransAndRotate(&trans, &rotate, mLocalTrans, false);
    mPosition.x = trans.x;
    mPosition.y = trans.y;
    mPosition.z = trans.z;
    mAppearController->storeCurrentPosture();
}

bool MercatorRotateParts::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    return mAppearController->receiveMsg(msg);
}

void MercatorRotateParts::control() {
    mRotator->movement();
} 

void MercatorRotateParts::calcAndSetBaseMtx() {
    TVec3f trans;
    TPos3f rotate;
    TMtx34f mtx;
    TPos3f baseMtx;
    baseMtx.identity();
    MR::makeMtxRotate(mtx.toMtxPtr(), mLocalRotate.x, mLocalRotate.y, mLocalRotate.z);
    baseMtx.concat(mRotator->getRotateMtx(), mtx);
    MR::convertMercatorPlaneToSphereTransAndRotate(&trans, &rotate, mLocalTrans, false);
    baseMtx.concat(rotate, baseMtx);
    baseMtx.mMtx[0][3] = trans.x;
    baseMtx.mMtx[1][3] = trans.y;
    baseMtx.mMtx[2][3] = trans.z;
    MR::setBaseTRMtx(this, baseMtx);
}

void MercatorRotateParts::init(const JMapInfoIter &rIter) {
    char objName[0x100];
    MR::getMapPartsObjectName(objName, sizeof(objName), rIter);
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoTrans(rIter, &mLocalTrans);
    MR::getJMapInfoRotate(rIter, &mLocalRotate);
    initModelManagerWithAnm(objName, nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    TVec3f bodyOffs;
    bodyOffs.x = 0.0f;
    bodyOffs.y = 0.0f;
    bodyOffs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 4, 0.0f, bodyOffs);
    MR::initCollisionParts(this, objName, getSensor("body"), nullptr);
    mRotator = new MapPartsRotator(this);
    mRotator->init(rIter);
    mRotator->start();
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

MercatorRotateParts::~MercatorRotateParts() {

}
