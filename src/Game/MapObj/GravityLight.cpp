#include "Game/MapObj/GravityLight.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/MapObj/SwingLight.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"

namespace {
    // const f32 sBaseInterpoleRate = 0.0f;
    // const f32 sAccel = 0.0f;
    // const f32 sSpeedMax = 0.0f;

    const GXColor color = {170, 170, 255, 140};
};  // namespace

namespace NrvGravityLight {
    NEW_NERVE(GravityLightNrvWait, GravityLight, Wait);
};  // namespace NrvGravityLight

GravityLight::GravityLight(const char* pName)
    : LiveActor(pName), mLightCylinder(), mMapPartsRailMover(), mMapPartsRotator(), mRadius(2500.0f), mCenter(0.0f, 0.0f, 0.0f), _D8() {
    mBaseMtx.identity();
}

void GravityLight::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::makeMtxTR(mBaseMtx, mPosition, mRotation);

    mRadius = 2500.0f * mScale.y;

    MR::connectToSceneMapObjMovementCalcAnim(this);

    const char* pName;
    MR::getObjectName(&pName, rIter);

    mLightCylinder = new LightCylinder(pName, getBaseMtx(), color);
    mLightCylinder->initLightCylinderByOwner(rIter);
    mLightCylinder->makeActorAppeared();

    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);

    f32 rotateSpeed = 0.0f;
    MR::getMapPartsArgRotateSpeed(&rotateSpeed, rIter);

    bool canRotate = rotateSpeed != 0.0f;
    bool isRailConnected = MR::isConnectedWithRail(rIter);

    if (isRailConnected) {
        initRailRider(rIter);

        mMapPartsRailMover = new MapPartsRailMover(this);
        mMapPartsRailMover->init(rIter);
        mMapPartsRailMover->start();
    }

    if (canRotate) {
        mMapPartsRotator = new MapPartsRotator(this);
        mMapPartsRotator->init(rIter);
        mMapPartsRotator->start();
    }

    if (isRailConnected || canRotate) {
        MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    }

    f32 boundingRadius;
    MR::calcModelBoundingRadius(&boundingRadius, mLightCylinder);

    boundingRadius += 0.5f * mRadius;

    TVec3f center(mPosition);
    if (isRailConnected) {
        f32 radius;
        MR::calcRailClippingInfo(&center, &radius, this, 100.0f, 500.0f);

        boundingRadius += radius;
    }

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    mCenter.scaleAdd(0.5f * mRadius, -upVec, mPosition);

    MR::setClippingTypeSphere(this, boundingRadius, &mCenter);
    MR::setGroupClipping(this, rIter, 16);

    initNerve(GET_NERVE(GravityLight, GravityLightNrvWait));

    makeActorAppeared();
}

void GravityLight::control() {
    if (mMapPartsRailMover != nullptr) {
        mMapPartsRailMover->movement();

        mPosition.set(mMapPartsRailMover->_28);

        MR::makeMtxTR(mBaseMtx, mPosition, mRotation);
    }

    if (mMapPartsRotator == nullptr) {
        mMapPartsRotator->movement();

        TPos3f baseMtx;
        baseMtx.identity();
        baseMtx.concat(mMapPartsRotator->getRotateMtx());

        mBaseMtx.set(baseMtx);
    }
}
