#include "Game/MapObj/ClipAreaMovable.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

void ClipAreaMovable_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

ClipAreaMovable::ClipAreaMovable(const char* pName) : ClipArea(pName), mMapPartsRailMover(), mMapPartsRotator() {
}

void ClipAreaMovable::init(const JMapInfoIter& rIter) {
    initBaseMatrix(rIter);
    MR::connectToScene(this, MR::MovementType_ClippedMapParts, -1, -1, MR::DrawType_ClipArea);
    MR::setGroupClipping(this, rIter, 0x40);
    MR::setClippingTypeSphere(this, MR::getMaxAbsElement(mScale) * 1400.0f);
    initMoveFunction(rIter);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::useStageSwitchReadB(this, rIter);
    MR::addBaseMatrixFollowTarget(this, rIter, 0, 0);
    makeActorAppeared();
}

void ClipAreaMovable::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    startMoveFunction();
}

void ClipAreaMovable::makeActorDead() {
    endMoveFunction();
    LiveActor::makeActorDead();
}

void ClipAreaMovable::control() {
    if (!MR::isValidSwitchB(this) || MR::isOnSwitchB(this)) {
        movementMoveFunction();
    }

    updateMatrix();
}

void ClipAreaMovable::initMoveFunction(const JMapInfoIter& rIter) {
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);

        mMapPartsRailMover = new MapPartsRailMover(this);
        mMapPartsRailMover->init(rIter);
    }

    mMapPartsRotator = new MapPartsRotator(this);
    mMapPartsRotator->init(rIter);
}

void ClipAreaMovable::startMoveFunction() {
    if (mMapPartsRotator != nullptr) {
        mMapPartsRotator->start();
    }

    if (mMapPartsRailMover != nullptr) {
        mMapPartsRailMover->start();
    }
}

void ClipAreaMovable::endMoveFunction() {
    if (mMapPartsRotator != nullptr) {
        mMapPartsRotator->end();
    }

    if (mMapPartsRailMover != nullptr) {
        mMapPartsRailMover->end();
    }
}

void ClipAreaMovable::movementMoveFunction() {
    if (mMapPartsRailMover != nullptr) {
        mMapPartsRailMover->movement();
    }

    if (mMapPartsRotator != nullptr) {
        mMapPartsRotator->movement();
    }
}

void ClipAreaMovable::updateMatrix() {
    TPos3f mtx;
    mtx.identity();

    if (mMapPartsRailMover != nullptr) {
        mPosition.set(mMapPartsRailMover->_28);
    }

    if (mMapPartsRotator != nullptr) {
        mtx.concat(mMapPartsRotator->getRotateMtx(), mtx);
    }

    mtx.setTrans(mPosition);
    mBaseMtx.set(mtx);
}

NameObj* MR::createClipAreaSphere(const char* pParam1) {
    ClipArea* pClipArea = new ClipAreaMovable(pParam1);

    MR::setClipSphereArea(pClipArea);

    return pClipArea;
}

NameObj* MR::createClipAreaCenterBox(const char* pParam1) {
    ClipArea* pClipArea = new ClipAreaMovable(pParam1);

    MR::setClipCenterBoxArea(pClipArea);

    return pClipArea;
}

NameObj* MR::createClipAreaBottomBox(const char* pParam1) {
    ClipArea* pClipArea = new ClipAreaMovable(pParam1);

    MR::setClipBottomBoxArea(pClipArea);

    return pClipArea;
}

NameObj* MR::createClipAreaTopCone(const char* pParam1) {
    ClipArea* pClipArea = new ClipAreaMovable(pParam1);

    MR::setClipTopConeArea(pClipArea);

    return pClipArea;
}

NameObj* MR::createClipAreaBottomCone(const char* pParam1) {
    ClipArea* pClipArea = new ClipAreaMovable(pParam1);

    MR::setClipBottomConeArea(pClipArea);

    return pClipArea;
}
