#include "Game/MapObj/MercatorRailMoveParts.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRailRotator.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"

MercatorRailMoveParts::MercatorRailMoveParts(const char *pName) : LiveActor(pName) {
    mRailMover = nullptr;
    mRailRotator = nullptr;
    mAppearController = nullptr;
    _E0.x = 0.0f;
    _E0.y = 0.0f;
    _E0.z = 0.0f;
    mIsNotMoving = false;
    mRotateMtx.identity();
}

void MercatorRailMoveParts::initAfterPlacement() {
    TVec3f trans;
    TPos3f rotate;
    MR::convertMercatorPlaneToSphereTransAndRotate(&trans, &rotate, mLocalTrans, false);
    mPosition.set(trans);
    
    if (MR::isValidSwitchB(this)) {
        mIsNotMoving = true;
    }
}

bool MercatorRailMoveParts::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (mAppearController->receiveMsg(msg)) {
        return true;
    }

    return MR::receiveMapPartsRotateMsg(this, msg, mRailMover, mRailRotator);
}

void MercatorRailMoveParts::startMove() {
    mIsNotMoving = false;
}

void MercatorRailMoveParts::endMove() {
    mIsNotMoving = true;
}

void MercatorRailMoveParts::control() {
    if (!mIsNotMoving) {
        mRailMover->movement();
        mRailRotator->movement();
        updatePose();
    }

    TVec3f trans;
    TPos3f rotate;
    MR::convertMercatorPlaneToSphereTransAndRotate(&trans, &rotate, mRailMover->_28, false);
    mPosition.set(trans);
}

void MercatorRailMoveParts::calcAndSetBaseMtx() {
    TVec3f trans;
    TPos3f rotate;
    MR::convertMercatorPlaneToSphereTransAndRotate(&trans, &rotate, mRailMover->_28, false);
    TPos3f baseMtx;
    baseMtx.identity();

    if (mRailRotator->isWorking()) {
        baseMtx.concat(mRailRotator->_5C, baseMtx);
    }
    else {
        baseMtx.set(mRotateMtx);
    }

    baseMtx.concat(rotate, baseMtx);
    baseMtx.mMtx[0][3] = mPosition.x;
    baseMtx.mMtx[1][3] = mPosition.y;
    baseMtx.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, baseMtx);
}

void MercatorRailMoveParts::updatePose() {
    TMtx34f mtx;
    mtx.identity();
    MR::makeMtxRotate(mtx.toMtxPtr(), mLocalRotation.x, mLocalRotation.y, mLocalRotation.z);

    if (mRailRotator->isWorking()) {
        mtx.concat(mRailRotator->_5C, mtx);
    }

    mRotateMtx.set(mtx);
}

void MercatorRailMoveParts::init(const JMapInfoIter &rIter) {
    char objName[0x100];
    MR::getMapPartsObjectName(objName, sizeof(objName), rIter);
    MR::initDefaultPosForMercator(this, rIter, false);
    MR::getJMapInfoTrans(rIter, &mLocalTrans);
    MR::getJMapInfoRotateLocal(rIter, &mLocalRotation);
    initModelManagerWithAnm(objName, nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);

    TVec3f what;
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 0.0f;
    sensorOffs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 4, 0.0f, sensorOffs);
    MR::initCollisionParts(this, "body", getSensor("body"), nullptr);

    if (MR::useStageSwitchReadB(this, rIter)) {
        void (MercatorRailMoveParts::*e)(void) = &MercatorRailMoveParts::startMove;
        void (MercatorRailMoveParts::*s)(void) = &MercatorRailMoveParts::endMove;
        MR::listenStageSwitchOnOffB(this, MR::Functor<MercatorRailMoveParts>(this, e), MR::Functor<MercatorRailMoveParts>(this, s));
    }

    initRailRider(rIter);
    what.setInlinePS_2(mPosition);
    mRailMover = MR::createMapPartsRailMoverForMercator(this, rIter, true);
    mRailMover->start();
    mRailRotator = new MapPartsRailRotator(this);
    mRailRotator->initWithRotateMtx(rIter, mRotateMtx.toMtxPtr());
    mRailRotator->start();
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

    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    MR::initMapPartsClipping(this, rIter, nullptr, true);
    updatePose();
}

MercatorRailMoveParts::~MercatorRailMoveParts() {

}
