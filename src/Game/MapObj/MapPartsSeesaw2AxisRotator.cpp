#include "Game/MapObj/MapPartsSeesaw2AxisRotator.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

namespace {
    const f32 sInertiaConstDefault = 500.0f;
    const f32 sRestoreForceDefault = 100.0f;
}  // namespace

namespace NrvMapPartsSeesaw2AxisRotator {
    NEW_NERVE(HostTypeWait, MapPartsSeesaw2AxisRotator, Wait);
    NEW_NERVE(HostTypeStay, MapPartsSeesaw2AxisRotator, Stay);
    NEW_NERVE(HostTypeMove, MapPartsSeesaw2AxisRotator, Move);
    NEW_NERVE(HostTypeHipDrop, MapPartsSeesaw2AxisRotator, HipDrop);
}  // namespace NrvMapPartsSeesaw2AxisRotator

MapPartsSeesaw2AxisRotator::MapPartsSeesaw2AxisRotator(LiveActor* pHost, const char* pMoveSound, f32 moveSoundSpeed)
    : MapPartsRotatorBase(pHost, "シーソー(2軸)"), mRotateAngle(), mInertiaConst(sInertiaConstDefault), mRestoreForce(sRestoreForceDefault),
      mUp(0.0f, 1.0f, 0.0f), mHipDrop(), mRotateSpeed(), mMoveSound(pMoveSound), mMoveSoundSpeed(moveSoundSpeed) {
    mRotateMtx.identity();
    mBaseMtx.identity();
    mBaseMtxInv.identity();
}

void MapPartsSeesaw2AxisRotator::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeWait));
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
    s32 inertia = 0;
    MR::getMapPartsArgRotateAccelType(&inertia, rIter);

    if (inertia > 0) {
        mInertiaConst = inertia;
    }

    s32 restoreForce = 0;
    MR::getMapPartsArgRotateStopTime(&restoreForce, rIter);
    mRestoreForce = restoreForce;
}

void MapPartsSeesaw2AxisRotator::start() {
    mBaseMtx.set(mHost->getBaseMtx());
    mBaseMtxInv.invert(mBaseMtx);
    mBaseMtx.getYDir(mUp);
    MR::normalize(&mUp);
    mRotateMtx.set(mBaseMtx);
    mRotateMtx.zeroTrans();
    setNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeStay));
}

void MapPartsSeesaw2AxisRotator::end() {
}

bool MapPartsSeesaw2AxisRotator::isWorking() const {
    return !isNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeWait));
}

bool MapPartsSeesaw2AxisRotator::receiveMsg(u32 msg) {
    if (msg == ACTMES_PLAYER_HIP_DROP_FLOOR) {
        if (!MR::isOnPlayer(MR::getBodySensor(mHost))) {
            return false;
        }

        mHipDrop = true;

        if (isNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeMove)) || isNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeStay))) {
            setNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeHipDrop));
            return true;
        }
    }

    return false;
}

void MapPartsSeesaw2AxisRotator::rotate() {
    TVec3f position(*MR::getPlayerPos());
    mBaseMtxInv.mult(position, position);
    f32 distance = position.length();
    TVec3f axis;
    axis.cross(mUp, position);
    MR::normalize(&axis);
    TPos3f tilt;
    tilt.identity();
    tilt.setRotate(axis, MR::toRadian(mRotateAngle));
    TPos3f target;
    target.concat(mBaseMtx, tilt);
    target.zeroTrans();

    f32 previousSpeed = mRotateSpeed;
    f32 speed = MR::clamp((0.001f * distance) / getInertiaConst(), 0.0f, 1.0f);
    mRotateSpeed = speed - previousSpeed;

    if (mMoveSound != nullptr && mMoveSoundSpeed < MR::abs(mRotateSpeed)) {
        MR::startLevelSound(mHost, mMoveSound);
    }

    MR::blendMtxRotate(mRotateMtx.toMtxPtr(), target.toMtxPtr(), speed, mRotateMtx.toMtxPtr());
}

void MapPartsSeesaw2AxisRotator::restoreMove() {
    f32 speed = MR::clamp(0.0001f * mRestoreForce, 0.0f, 1.0f);
    TPos3f target;
    target.set(mBaseMtx);
    target.zeroTrans();
    MR::blendMtxRotate(mRotateMtx.toMtxPtr(), target.toMtxPtr(), speed, mRotateMtx.toMtxPtr());
}

f32 MapPartsSeesaw2AxisRotator::getInertiaConst() const {
    if (isNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeHipDrop))) {
        return 0.1f * mInertiaConst;
    }

    return mInertiaConst;
}

void MapPartsSeesaw2AxisRotator::exeMove() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        rotate();
    }

    restoreMove();
}

void MapPartsSeesaw2AxisRotator::exeHipDrop() {
    if (!mHipDrop) {
        setNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeMove));
        mHipDrop = false;
        return;
    }

    rotate();
    mHipDrop = false;
}

void MapPartsSeesaw2AxisRotator::exeStay() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        setNerve(GET_NERVE(MapPartsSeesaw2AxisRotator, HostTypeMove));
    }
}

MapPartsSeesaw2AxisRotator::~MapPartsSeesaw2AxisRotator() {
}
