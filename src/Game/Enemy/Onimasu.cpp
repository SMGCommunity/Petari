#include "Game/Enemy/Onimasu.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void Onimasu_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.000003814697265625f;
    (void)0.0f;
    (void)2.0f;
    (void)1.5707963705062866f;
    (void)-1.0f;
    (void)0.5f;
    (void)3.1415927410125732f;
}

namespace {
    static const s32 sWaitTime = 90;
    static const s32 sTimeToNextPoint = 30;
    static const f32 sGravityScalar = 1.6f;
    // static const f32 sQuickRatio = ;
    static const s32 sWaitTimeQuick = 45;
    static const s32 sTimeToNextPointQuick = 15;
    static const f32 sGravityScalarQuick = 6.4f;
    static const f32 sCamShakeDistanceStrong = 2000.0f;
    static const f32 sCamShakeDistanceWeak = 3500.0f;
    static const f32 sSensorRadius = 800.0f;
}  // namespace

namespace NrvOnimasu {
    NEW_NERVE(HostTypeWaitForSwitchOn, Onimasu, WaitForSwitchOn);
    NEW_NERVE(HostTypeWait, Onimasu, Wait);
    NEW_NERVE(HostTypeWaitForStamp, Onimasu, Wait);
    NEW_NERVE(HostTypeJump, Onimasu, Move);
    NEW_NERVE(HostTypeStamp, Onimasu, Stamp);
}  // namespace NrvOnimasu

Onimasu::~Onimasu() {
}

Onimasu::Onimasu(const char* pName) : LiveActor(pName), _BC(1.0f, 0.0f, 0.0f), _F8(gZeroVec) {
    _8C.identity();
    _C8.set(0.0f, 0.0f, 0.0f, 1.0f);
    _D8.set(0.0f, 0.0f, 0.0f, 1.0f);
    _E8.set(0.0f, 0.0f, 0.0f, 1.0f);
}

void Onimasu::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    initFromRailPoint();
    initModelManagerWithAnm("Onimasu", nullptr, false);

    MR::connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(this);

    MR::initLightCtrl(this);
    initEffectKeeper(0, nullptr, false);

    MR::setEffectHostMtx(this, "Move", _8C);

    initSound(4, false);

    initHitSensor(1);
    MR::initCollisionParts(this, "Onimasu", MR::addHitSensor(this, "body", ATYPE_MAP_OBJ_PRESS, 8, ::sSensorRadius, TVec3f(0.0f, 0.0f, 0.0f)),
                           nullptr);

    MR::initAndSetRailClipping(&_F8, this, 100.0f, 500.0f);

    MR::setGroupClipping(this, rIter, 16);

    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);

    MR::onCalcGravity(this);

    if (MR::useStageSwitchReadA(this, rIter)) {
        initNerve(GET_NERVE(Onimasu, HostTypeWaitForSwitchOn));
    } else {
        initNerve(GET_NERVE(Onimasu, HostTypeWait));
    }

    MR::useStageSwitchSleep(this, rIter);

    makeActorAppeared();
}

void Onimasu::initAfterPlacement() {
    collectRailPointInfo();
    MR::moveCoordAndTransToRailStartPoint(this);
    MR::setRailDirectionToEnd(this);

    TPos3f mtx;
    mtx.identity();
    MR::makeMtxTR(mtx.toMtxPtr(), this);
    MR::setBaseTRMtx(this, mtx);

    if (MR::isEqualStageName("FactoryGalaxy")) {
        mtx.getXDir(_BC);
        MR::normalize(&_BC);
    }

    mtx.getQuat(_C8);
    _D8.set(_C8);
    _E8.set(_D8);

    if (MR::isEqualStageName("FactoryGalaxy")) {
        f32 turnDirection = 0.0f;
        if (calcTurnDirection(&turnDirection) && turnDirection == -1.0f) {
            setNerve(GET_NERVE(Onimasu, HostTypeWaitForStamp));
        }
    }
}

void Onimasu::calcTargetPose() {
    TQuat4f rotationChange;
    rotationChange.set(0.0f, 0.0f, 0.0f, 1.0f);

    if (MR::isSameDirection(getLastPointNormal(), getNextPointNormal())) {
        f32 turnDirection = 0.0f;
        if (calcTurnDirection(&turnDirection)) {
            TPos3f mtx;
            mtx.identity();
            const f32 pi = MR::pi();
            mtx.makeRotate(getLastPointNormal(), pi * turnDirection);
            mtx.mult(_BC, _BC);

            MR::normalize(&_BC);
        } else {
            TVec3f lastPointPos;
            MR::calcRailPointPos(&lastPointPos, this, getLastPointNo());

            TVec3f nextPointPos;
            MR::calcRailPointPos(&nextPointPos, this, getNextPointNo());

            TVec3f railDir(nextPointPos - lastPointPos);
            MR::normalize(&railDir);

            TVec3f down;
            calcGravityDir(&down);
            _BC.cross(railDir, down);
            MR::normalize(&_BC);
        }
    } else {
        TQuat4f normalRotation;
        normalRotation.set(0.0f, 0.0f, 0.0f, 1.0f);
        normalRotation.setRotate(getLastPointNormal(), getNextPointNormal());
        rotationChange.set(normalRotation);
        normalRotation.getRotate(_BC);
        MR::normalize(&_BC);
    }

    _C8.set(_D8);

    TQuat4f quarterTurn;
    quarterTurn.set(0.0f, 0.0f, 0.0f, 1.0f);
    quarterTurn.setRotate(_BC, MR::pi() / 2.0f);
    rotationChange.mult(quarterTurn);
    _D8.mult(rotationChange);

    TVec3f startFront;
    _C8.getZDir(startFront);

    TVec3f targetFront;
    _D8.getZDir(targetFront);

    if (MR::isSameDirection(startFront, targetFront)) {
        TQuat4f adjustment;
        adjustment.setRotate(_BC, MR::pi() * 0.001f);
        _C8.mult(adjustment);
    }
}

void Onimasu::updatePose() {
    f32 nerveRate = static_cast< f32 >(getNerveStep()) / getTimeToNextPoint();
    if (1.0f < nerveRate) {
        nerveRate = 1.0f;
    }

    _E8.set(_C8);
    _E8.slerp(_D8, nerveRate);
    _E8.normalize();
}

void Onimasu::calcAndSetBaseMtx() {
    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.setQT(_E8, mPosition);

    MR::setBaseTRMtx(this, baseMtx);
}

void Onimasu::land() {
    emitEffectLand();
    MR::startRumbleWithShakeCameraNormalWeak(this, "中", "弱", ::sCamShakeDistanceStrong, ::sCamShakeDistanceWeak);

    TVec3f railPointPos;
    MR::calcRailPointPos(&railPointPos, this, getNextPointNo());

    mPosition.set(railPointPos);
    mVelocity.zero();

    MR::moveCoordToRailPoint(this, getNextPointNo());
}

bool Onimasu::calcTurnDirection(f32* pTurnDir) const {
    s32 arg0 = -1;
    MR::getCurrentRailPointArg0NoInit(this, &arg0);

    if (arg0 == -1) {
        return false;
    }

    if (arg0 == 0) {
        *pTurnDir = 0.5f;
        return true;
    }

    if (arg0 == 1) {
        *pTurnDir = -0.5f;
        return true;
    }

    if (arg0 == 2) {
        *pTurnDir = -1.0f;
        return true;
    }

    return false;
}

void Onimasu::updateStompVelocity() {
    f32 speed = getGravityScalar() * getTimeToNextPoint() * 0.5f;

    TVec3f gravityDir;
    gravityDir.set(-getNextPointNormal());
    mVelocity.set(-gravityDir * speed);
}

void Onimasu::calcGravityDir(TVec3f* pDir) const {
    pDir->set(-getNextPointNormal());
}

f32 Onimasu::getGravityScalar() const {
    return MR::isGalaxyQuickCometAppearInCurrentStage() ? ::sGravityScalarQuick : ::sGravityScalar;
}

s32 Onimasu::getTimeToNextPoint() const {
    bool quick = MR::isGalaxyQuickCometAppearInCurrentStage();
    return quick ? ::sTimeToNextPointQuick : ::sTimeToNextPoint;
}

s32 Onimasu::getWaitTime() const {
    bool quick = MR::isGalaxyQuickCometAppearInCurrentStage();
    return quick ? ::sWaitTimeQuick : ::sWaitTime;
}

void Onimasu::emitEffectLand() {
    TVec3f railPointPos;
    MR::calcRailPointPos(&railPointPos, this, getNextPointNo());

    railPointPos.add((-getNextPointNormal() * ::sSensorRadius) * 0.5f);

    if (MR::isSameDirection(_BC, getNextPointNormal())) {
        MR::makeMtxUpNoSupportPos(&_8C, getNextPointNormal(), railPointPos);
    } else {
        MR::makeMtxUpSidePos(&_8C, getNextPointNormal(), _BC, railPointPos);
    }

    MR::emitEffect(this, "Move");
    MR::startSound(this, "SE_OJ_ONIMASU_LAND");
}

void Onimasu::exeWaitForSwitchOn() {
    if (MR::isOnSwitchA(this)) {
        f32 turnDirection = 0.0f;
        if (calcTurnDirection(&turnDirection) && turnDirection == -1.0f) {
            setNerve(GET_NERVE(Onimasu, HostTypeWaitForStamp));
        } else {
            setNerve(GET_NERVE(Onimasu, HostTypeWait));
        }
    }
}

void Onimasu::exeWait() {
    if (MR::isStep(this, getWaitTime())) {
        if (isNerve(GET_NERVE(Onimasu, HostTypeWaitForStamp))) {
            setNerve(GET_NERVE(Onimasu, HostTypeStamp));
        } else {
            setNerve(GET_NERVE(Onimasu, HostTypeJump));
        }
    }
}

void Onimasu::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ONIMASU_JUMP");

        incrementNextPoint();
        calcTargetPose();
        startMoveInner();
    }

    if (MR::isStep(this, 1)) {
        MR::offCalcGravity(this);
    }

    updatePose();
    updatePoseInner();

    if (MR::isStep(this, getTimeToNextPoint())) {
        land();

        f32 turnDirection = 0.0f;
        if (calcTurnDirection(&turnDirection) && turnDirection == -1.0f) {
            setNerve(GET_NERVE(Onimasu, HostTypeWaitForStamp));
        } else {
            setNerve(GET_NERVE(Onimasu, HostTypeWait));
        }
    }
}

void Onimasu::exeStamp() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ONIMASU_JUMP");
        updateStompVelocity();
    }

    TVec3f gravityDir;
    calcGravityDir(&gravityDir);

    TVec3f& rVelocity = mVelocity;
    rVelocity.add(gravityDir * getGravityScalar());

    if (MR::isStep(this, getTimeToNextPoint())) {
        land();
        setNerve(GET_NERVE(Onimasu, HostTypeWait));
    }
}

namespace OnimasuFunction {
    bool getPolygonOnRailPoint(TVec3f* pPosition, TVec3f* pNormal, const LiveActor* pActor, int pointNo) {
        TVec3f railPosition;
        MR::calcRailPointPos(&railPosition, pActor, pointNo);

        TVec3f gravity(0.0f, 1.0f, 0.0f);
        MR::calcGravityVector(pActor, railPosition, &gravity, nullptr, 0);
        TVec3f direction(gravity);
        gravity.setLength(2000.0f);

        Triangle triangle;
        if (!MR::getFirstPolyOnLineToMapExceptSensor(pPosition, &triangle, railPosition, gravity, pActor->getSensor("body"))) {
            return false;
        }

        pNormal->set(*triangle.getFaceNormal());
        return true;
    }
}  // namespace OnimasuFunction
