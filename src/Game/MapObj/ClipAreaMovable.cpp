#include "Game/MapObj/ClipAreaMovable.hpp"
#include "Game/MapObj/ClipArea.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"

ClipAreaMovable::ClipAreaMovable(const char* pName) : ClipArea(pName) {
    mMapPartsRailMover = 0;
    mMapPartsRotator = 0;
}

void ClipAreaMovable::init(const JMapInfoIter& rIter) {
    initBaseMatrix(rIter);
    MR::connectToScene(this, MR::MovementType_ClippedMapParts, -1, -1, MR::DrawType_ClipArea);
    MR::setGroupClipping(this, rIter, 0x40);
    f32 f = MR::getMaxAbsElement(mScale);
    MR::setClippingTypeSphere(this, f * 1400.0f);
    initMoveFunction(rIter);
    initHitSensor(1);
    TVec3f stack_8 = TVec3f(0.0f, 0.0f, 0.0f);
    MR::addHitSensorMapObj(this, "body", 0, 0.0f, stack_8);
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
    if (MR::isValidSwitchB(this) && MR::isOnSwitchB(this)) {
        movementMoveFunction();
        updateMatrix();
    }
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
    if (mMapPartsRotator)
        mMapPartsRotator->start();
    if (mMapPartsRailMover)
        mMapPartsRailMover->start();
}

void ClipAreaMovable::endMoveFunction() {
    if (mMapPartsRotator)
        mMapPartsRotator->end();
    if (mMapPartsRailMover)
        mMapPartsRailMover->end();
}

void ClipAreaMovable::movementMoveFunction() {
    if (mMapPartsRailMover)
        mMapPartsRailMover->movement();
    if (mMapPartsRotator)
        mMapPartsRotator->movement();
}

void ClipAreaMovable::updateMatrix() {
    TMtx34f mtx;
    mtx.identity();
    if (mMapPartsRailMover) {
        mPosition.set(mMapPartsRailMover->_28);
    }

    if (mMapPartsRotator)
        mtx.concat(mMapPartsRotator->getRotateMtx(), mtx);

    register ClipAreaMovable* regThis = this;
    register TMtx34f* regMtx = &mtx;

    mtx[0][3] = mPosition.x;
    mtx[1][3] = mPosition.y;
    mtx[2][3] = mPosition.z;
    mBaseMatrix.setInline(mtx);
}
