#include "Game/Enemy/StinkBugBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JMath/JMath.hpp"

StinkBugBase::StinkBugBase(const char* pArg)
    : LiveActor(pArg), _8C(0.0f, 0.0f, 1.0f), _98(0.0f, 0.0f, 0.0f), _A4(0.0f, 0.0f, 1.0f), _B0(180.0f), _B4(1000.0f), _B8(false) {}

void StinkBugBase::setDashVelocity(f32 velocity) {
    TVec3f result;
    f32 f1 = _B4;
    JMAVECScaleAdd(&_8C, &_98, &result, f1);
    f32 distance = PSVECDistance(&result, &mPosition);
    // Illogical branching in the ASM. Possible inline?
    if (velocity >= distance) {
        distance = velocity;
    }

    mVelocity.scale(distance, _8C);
}

void StinkBugBase::fixInitPos() {
    mVelocity.z = 0.0f;
    mVelocity.y = 0.0f;
    mVelocity.x = 0.0f;
    mPosition.set(_98);
}

void StinkBugBase::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* buff;
    MR::getObjectName(&buff, rIter);
    initModelManagerWithAnm(buff, nullptr, false);
    MR::connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(this);
    MR::initLightCtrl(this);
    MR::initShadowFromCSV(this, "Shadow");
    initEffectKeeper(2, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, 0);
    MR::useStageSwitchWriteDead(this, rIter);
    _98.set(mPosition);
    MR::calcFrontVec(&_8C, this);
    _A4.set(_8C);
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    JMathInlineVEC::PSVECNegate(&upVec, &mGravity);
}

bool StinkBugBase::isPlayerInTerritory(f32 arg1, f32 arg2, f32 arg3, f32 arg4) const {
    if (!MR::isNearPlayer(this, _B4 + arg1)) {
        return false;
    }

    TVec3f v1;
    TVec3f* playerPos = MR::getPlayerPos();
    // r5 goes through r31 before MR::getPlayerPos()
    v1.sub(*playerPos, mPosition);

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    f32 dot = v1.dot(upVec);

    if (__fabs(dot) > arg3) {
        return false;
    }

    TVec3f sideVec;
    MR::calcSideVec(&sideVec, this);

    dot = v1.dot(sideVec);

    if (__fabs(dot) > arg2) {
        return false;
    }

    if (_B0 == 0.0f) {
        return v1.dot(_8C) > 0.0f;
    }

    if (_B0 == 180.0f) {
        return true;
    }

    f32 f1 = __fabsf(JMath::sSinCosTable.sinLapRad(_B0));

    TVec3f scaledAdded;
    // r3 and r4's assembly are in the wrong order
    JMAVECScaleAdd(&_A4, &mPosition, &scaledAdded, (-arg4) / f1);

    TVec3f v3;
    v3.sub(*MR::getPlayerPos(), scaledAdded);

    const TVec3f* gravity = &mGravity;
    f32 f2 = -gravity->dot(v3);

    TVec3f v5;
    JMAVECScaleAdd(gravity, &v3, &v5, f2);

    return MR::isNearAngleDegree(v5, _A4, _B0);
}

bool StinkBugBase::tryTurnSearch(f32 arg) {
    if (_B8) {
        MR::rotateVecDegree(&_8C, mGravity, arg);
    } else {
        MR::rotateVecDegree(&_8C, mGravity, -arg);
    }

    if (!MR::isNearAngleDegree(_8C, _A4, _B0)) {
        MR::clampVecAngleDeg(&_8C, _A4, _B0);
        _B8 = !_B8;
        return true;
    }

    return false;
}

bool StinkBugBase::tryTurnDashSign(f32 arg) {
    TVec3f* playerPos = MR::getPlayerPos();
    MR::turnDirectionToTargetUseGroundNormalDegree(this, &_8C, *playerPos, arg);
    MR::clampVecAngleDeg(&_8C, _A4, _B0);
    return true;
}

bool StinkBugBase::isHitHorn(HitSensor* pSensor1, HitSensor* pSensor2, f32 arg3) const {
    TVec3f sensorDistance;
    sensorDistance.sub(pSensor2->mPosition, pSensor1->mPosition);

    if (sensorDistance.dot(_8C) < 0.0f) {
        return false;
    }

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    TVec3f v2;
    v2.scale(upVec.dot(sensorDistance), upVec);

    f32 radius = pSensor2->mRadius;
    return v2.length() <= arg3 + radius;
}
