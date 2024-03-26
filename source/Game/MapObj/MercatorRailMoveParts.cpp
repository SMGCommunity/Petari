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

// MercatorRailMoveParts::calcAndSetBaseMtx
// MercatorRailMoveParts::updatePose
// MercatorRailMoveParts::init

MercatorRailMoveParts::~MercatorRailMoveParts() {

}
