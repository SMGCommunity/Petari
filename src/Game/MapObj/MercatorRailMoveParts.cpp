#include "Game/MapObj/MercatorRailMoveParts.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRailRotator.hpp"
#include "Game/Util.hpp"

MercatorRailMoveParts::MercatorRailMoveParts(const char* pName) : LiveActor(pName) {
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
    mPosition.set< f32 >(trans);

    if (MR::isValidSwitchB(this)) {
        mIsNotMoving = true;
    }
}

bool MercatorRailMoveParts::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
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
    mPosition.set< f32 >(trans);
}

void MercatorRailMoveParts::calcAndSetBaseMtx() {
    TVec3f trans;
    TPos3f rotate;
    MR::convertMercatorPlaneToSphereTransAndRotate(&trans, &rotate, mRailMover->_28, false);
    TPos3f baseMtx;
    baseMtx.identity();

    if (mRailRotator->isWorking()) {
        baseMtx.concat(mRailRotator->_5C, baseMtx);
    } else {
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

void MercatorRailMoveParts::init(const JMapInfoIter& rIter) {
    char objName[256];
    MR::getMapPartsObjectName(objName, sizeof(objName), rIter);
    MR::initDefaultPosForMercator(this, rIter, false);
    MR::getJMapInfoTrans(rIter, &mLocalTrans);
    MR::getJMapInfoRotateLocal(rIter, &mLocalRotation);
    initModelManagerWithAnm(objName, nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);

    MR::addHitSensorMapObj(this, "body", 4, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, objName, getSensor("body"), nullptr);

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnOffB(this, MR::Functor(this, &MercatorRailMoveParts::startMove), MR::Functor(this, &MercatorRailMoveParts::endMove));
    }

    initRailRider(rIter);
    TVec3f what;
    what = mPosition;
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
    } else {
        makeActorAppeared();
    }

    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    MR::initMapPartsClipping(this, rIter, nullptr, true);
    updatePose();
}

MercatorRailMoveParts::~MercatorRailMoveParts() {
}
