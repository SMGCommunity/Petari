#include "Game/Map/Butterfly.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

void Butterfly_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)2.0f;
}

namespace NrvButterfly {
    NEW_NERVE(HostTypeWait, Butterfly, Wait);
    NEW_NERVE(HostTypeRunAway, Butterfly, RunAway);
    NEW_NERVE(HostTypeHive, Butterfly, Hive);
    NEW_NERVE(HostTypePerchOn, Butterfly, PerchOn);
    NEW_NERVE(HostTypeGotoSleepingMario, Butterfly, GotoSleepingMario);
    NEW_NERVE(HostTypeReadyToPerchOnSleepingMario, Butterfly, ReadyToPerchOnSleepingMario);
    NEW_NERVE(HostTypePerchOnSleepingMario, Butterfly, PerchOnSleepingMario);
};  // namespace NrvButterfly

namespace {
    static const f32 sRadius = 30.0f;
    static const f32 sDpdPointRadius = 200.0f;
    static const f32 sDpdPointRadiusFast = 100.0f;
    static const f32 sDpdSpeedFast = 3.0f;
    static const f32 sAppearStarPeaceScreenRadius = 10.0f;
    static const f32 sWaitAnimRate = 1.0f;
    static const f32 sHomeRadius = 200.0f;
    static const f32 sRunAwayVel = 12.0f;
    static const f32 sRunAwayTime = 30.0f;
    static const f32 sRunAwayAttenVel = 0.97f;
    static const f32 sRunAwayAnimRate = 3.0f;
    static const f32 sBackToHomeAccel = 0.01f;
    static const f32 sBackToHomeVelMax = 2.0f;
    static const f32 sBackToHomeAttenVel = 0.99f;
    static const f32 sMoveDistanceMax = 500.0f;
    static const f32 sHiveRadius = 100.0f;
    static const f32 sHiveAccel = 0.05f;
    static const f32 sHiveVelMax = 2.5f;
    static const f32 sHiveAnimRate = 1.2f;
    static const f32 sPerchOnAnimRate = 0.7f;
    static const f32 sAnimInterpolateRate = 0.02f;
    static const f32 sSleepingMarioNearDistance = 500.0f;
    static const f32 sPerchOnNearDistance = 10.0f;
    static const f32 sGotoSleepingMarioWaitTime = 300.0f;
    static const f32 sGotoSleepingMarioSpeedMax = 2.0f;
    static const f32 sReadyToPerchOnAnimRate = 0.5f;
    static const f32 sTransBlendRatio = 0.7f;
    static const f32 sTurnLimit = 0.997f;  // ~4.44 degrees
    static const f32 sUpdatePostureMinVel = 0.5f;

    static const Vec sMarioCapPosOffset = {0.0f, 50.2f, 24.9f};

    f32 getScreenDistanceToStarPointer(const LiveActor* pActor, s32 port) {
        TVec2f screenPos;
        MR::calcScreenPosition(&screenPos, pActor->mPosition);
        TVec2f pointerScreenPos;
        pointerScreenPos.set(*MR::getStarPointerScreenPosition(port));
        return screenPos.distance(pointerScreenPos);
    }
};  // namespace

Butterfly::Butterfly(const char* pName) : LiveActor(pName), mHomePos(gZeroVec), mColorIndex(), mPerchTarget(), mStarPieceAppeared() {
    mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
}

void Butterfly::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mHomePos.set(mPosition);
    initModelManagerWithAnm("Butterfly", nullptr, false);
    MR::connectToSceneMapObj(this);

    // Colors are stored in an animation for the purposes of easy indexing
    MR::getJMapInfoArg0NoInit(rIter, &mColorIndex);
    MR::startBrk(this, "Butterfly");
    MR::setBrkFrameAndStop(this, mColorIndex);

    initHitSensor(1);
    MR::addHitSensorMtxAnimal(this, "body", 8, ::sRadius, MR::getJointMtx(this, "buttBody"), TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(::sRadius, 0.0f, 0);
    initSound(2, false);

    s32 arg1 = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);
    if (arg1 == -1) {
        MR::declareStarPiece(this, 1);
    } else {
        mStarPieceAppeared = true;
    }

    MR::initStarPointerTarget(this, ::sDpdPointRadiusFast);
    MR::setClippingTypeSphere(this, ::sRadius);
    MR::setClippingFar50m(this);
    initNerve(&NrvButterfly::HostTypeWait::sInstance);
    makeActorAppeared();
    MR::startBck(this, "Butterfly", nullptr);
    MR::setBckFrameAtRandom(this);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
}

void Butterfly::initAfterPlacement() {
    if (MR::isEqualStageName("HeavensDoorGalaxy")) {
        TVec3f gravity;
        MR::calcGravityVector(this, &gravity, nullptr, 0);
        MR::makeQuatUpNoSupport(&mQuat, -gravity);
    } else {
        mQuat.setEuler(DEG_TO_RAD(mRotation));
    }
}

void Butterfly::control() {
    updatePosture();
    s32 port = 0;
    if (!MR::isStarPointerInScreenAnyPort(&port)) {
        return;
    }

    if (MR::getStarPointerScreenSpeed(port) < ::sDpdSpeedFast) {
        MR::setStarPointerTargetRadius3d(this, ::sDpdPointRadius);
    } else {
        MR::setStarPointerTargetRadius3d(this, ::sDpdPointRadiusFast);
    }
    tryAppearStarPeace(port);
}

void Butterfly::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.setQT(mQuat, mPosition);
    MR::setBaseTRMtx(this, mtx);
}

void Butterfly::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorNpc(pReceiver)) {
        return;
    }

    if (!isNerve(&NrvButterfly::HostTypeHive::sInstance)) {
        return;
    }

    mPerchTarget = pReceiver;
    setNerve(&NrvButterfly::HostTypePerchOn::sInstance);
}

void Butterfly::updatePosture() {
    TVec3f gravity;
    MR::calcGravityVector(this, &gravity, nullptr, 0);

    TVec3f up;
    mQuat.getYDir(up);
    TVec3f forward;
    mQuat.getZDir(forward);

    // Unused
    {
        TVec3f tmp = -gravity;
        TQuat4f quat;
        quat.setRotate(up, tmp, 1.0f);
    }

    TVec3f vel = mVelocity;
    if (vel.length() < ::sUpdatePostureMinVel) {
        mQuat.normalize();
        return;
    }

    MR::vecKillElement(vel, gravity, &vel);
    if (MR::isNearZero(vel)) {
        mQuat.normalize();
        return;
    }

    MR::normalize(&vel);
    TVec3f targetForward = vel.killElement(up);
    MR::normalizeOrZero(&targetForward);
    if (!MR::isNearZero(targetForward)) {
        TVec3f newForward;
        MR::turnVecToVecCos(&newForward, forward, targetForward, ::sTurnLimit, up);
        TQuat4f tmp;
        tmp.setRotate(forward, newForward, 1.0f);
        mQuat.mult(tmp);
    }
    mQuat.normalize();
}

void Butterfly::addRunAwayVelocity() {
    TVec2f pointerScreenPos;
    pointerScreenPos.set(*MR::getStarPointerScreenPosition(*MR::getStarPointerLastPointedPort(this)));
    TVec2f screenPos;
    MR::calcScreenPosition(&screenPos, mPosition);
    TVec2f runAwayDir = screenPos - pointerScreenPos;

    if (!MR::isNearZero(runAwayDir)) {
        MR::normalize(&pointerScreenPos);
    }

    TVec3f xDir = MR::getCamXdir();
    TVec3f invYDir = -MR::getCamYdir();

    TVec3f vel = xDir * runAwayDir.x + invYDir * runAwayDir.y;

    TVec3f gravity;
    MR::calcGravityVector(this, &gravity, nullptr, 0);

    if (0.0f < vel.dot(gravity)) {
        MR::vecKillElement(vel, gravity, &vel);
    }

    vel.setLength(::sRunAwayVel);
    mVelocity += vel;
}

bool Butterfly::tryRunAway() {
    s32 port = *MR::getStarPointerLastPointedPort(this);
    if (MR::isStarPointerInScreen(port) == 0) {
        return false;
    }

    if (!MR::isStarPointerPointing2P(this, nullptr, false, false)) {
        return false;
    }

    if (MR::testCorePadButtonB(port)) {
        return false;
    }

    if (::sMoveDistanceMax < mPosition.distance(mHomePos)) {
        return false;
    }

    TVec2f screenPos;
    MR::calcScreenPosition(&screenPos, mPosition);
    TVec2f pointerScreenPos;
    pointerScreenPos.set(*MR::getStarPointerScreenPosition(port));
    if (MR::getStarPointerScreenVelocity(port)->dot(screenPos - pointerScreenPos) <= 0.0f) {
        return false;
    }

    if (!MR::tryStarPointerCheckWithoutRumble(this, false)) {
        return false;
    }

    addRunAwayVelocity();
    setNerve(&NrvButterfly::HostTypeRunAway::sInstance);
    return true;
}

bool Butterfly::tryHive() {
    s32 port = 0;
    if (!MR::isStarPointerInScreenAnyPort(&port)) {
        return false;
    }

    if (::sMoveDistanceMax < mPosition.distance(mHomePos)) {
        return false;
    }

    TVec2f screenPos;
    TVec2f pointerScreenPos = *MR::getStarPointerScreenPosition(port);
    MR::calcScreenPosition(&screenPos, mPosition);
    if (::sHiveRadius < pointerScreenPos.distance(screenPos)) {
        return false;
    }

    if (!MR::testCorePadButtonB(port)) {
        return false;
    }

    setNerve(&NrvButterfly::HostTypeHive::sInstance);
    return true;
}

bool Butterfly::tryPerchOnSleepingMario() {
    if (::sSleepingMarioNearDistance < MR::calcDistanceToPlayer(this)) {
        return false;
    }

    if (!MR::isPlayerSleeping()) {
        return false;
    }

    setNerve(&NrvButterfly::HostTypeGotoSleepingMario::sInstance);
    return true;
}

bool Butterfly::tryAppearStarPeace(s32 port) {
    if (mStarPieceAppeared) {
        return false;
    }

    f32 distance;
    if (!MR::calcStarPointerScreenDistanceToTarget(this, &distance, port)) {
        return false;
    }

    if (::sAppearStarPeaceScreenRadius < distance) {
        return false;
    }

    if (!MR::appearStarPiece(this, mPosition, 1, ::sAppearStarPeaceScreenRadius, 40.0f, false)) {
        return false;
    }

    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    mStarPieceAppeared = true;
    return true;
}

void Butterfly::exeWait() {
    if (tryRunAway()) {
        return;
    }

    if (tryHive()) {
        return;
    }

    if (tryPerchOnSleepingMario()) {
        return;
    }

    MR::setBckRate(this, MR::converge(MR::getBckRate(this), ::sWaitAnimRate, ::sAnimInterpolateRate));
    mVelocity.x *= ::sBackToHomeAttenVel;
    mVelocity.y *= ::sBackToHomeAttenVel;
    mVelocity.z *= ::sBackToHomeAttenVel;

    TVec3f backToHomeVel = mHomePos - mPosition;
    if (MR::isNearZero(backToHomeVel)) {
        return;
    }

    MR::normalize(&backToHomeVel);
    mVelocity.add(backToHomeVel * ::sBackToHomeAccel);

    if (::sBackToHomeVelMax < mVelocity.length()) {
        mVelocity.setLength(::sBackToHomeVelMax);
    }

    if (!MR::isInvalidClipping(this)) {
        return;
    }

    if (mHomePos.distance(mPosition) < ::sHomeRadius) {
        MR::validateClipping(this);
    }
}

void Butterfly::exeRunAway() {
    if (MR::isFirstStep(this) && !MR::isInvalidClipping(this)) {
        MR::invalidateClipping(this);
    }

    MR::setBckRate(this, MR::converge(MR::getBckRate(this), ::sRunAwayAnimRate, ::sAnimInterpolateRate));
    mVelocity.x *= ::sRunAwayAttenVel;
    mVelocity.y *= ::sRunAwayAttenVel;
    mVelocity.z *= ::sRunAwayAttenVel;

    if (MR::isStep(this, ::sRunAwayTime)) {
        setNerve(&NrvButterfly::HostTypeWait::sInstance);
    }
}

void Butterfly::exeHive() {
    if (tryRunAway()) {
        return;
    }

    s32 port = 0;
    if (!MR::isStarPointerInScreenAnyPort(&port)) {
        setNerve(&NrvButterfly::HostTypeWait::sInstance);
        return;
    }

    if (::sMoveDistanceMax < mPosition.distance(mHomePos)) {
        setNerve(&NrvButterfly::HostTypeWait::sInstance);
        return;
    }

    TVec3f pos;
    MR::calcStarPointerWorldPointingPos(&pos, mPosition, port);
    TVec3f vel = pos - mPosition;
    mVelocity += vel * ::sHiveAccel;

    if (::sHiveVelMax < mVelocity.length()) {
        mVelocity.setLength(::sHiveVelMax);
    }

    MR::setBckRate(this, MR::converge(MR::getBckRate(this), ::sHiveAnimRate, ::sAnimInterpolateRate));
}

void Butterfly::exePerchOn() {
    HitSensor* bodySensor = getSensor("body");
    if (MR::isNear(bodySensor, mPerchTarget, bodySensor->getRadius() + mPerchTarget->getRadius())) {
        mVelocity.zero();
        MR::setBckRate(this, MR::converge(MR::getBckRate(this), ::sPerchOnAnimRate, ::sAnimInterpolateRate));
    } else {
        // PerchOn can only happen from Hive
        // This runs the Hive nerve until the perch target is near
        TVec3f vel = mPerchTarget->getPosition() - mPosition;
        vel.setLength(::sHiveAccel);
        mVelocity += vel;

        if (::sHiveVelMax < mVelocity.length()) {
            mVelocity.setLength(::sHiveVelMax);
        }

        MR::setBckRate(this, MR::converge(MR::getBckRate(this), ::sHiveAnimRate, ::sAnimInterpolateRate));
    }
    tryRunAway();
}

void Butterfly::exeGotoSleepingMario() {
    if (MR::isLessStep(this, ::sGotoSleepingMarioWaitTime)) {
        return;
    }

    TPos3f jointMtx;
    MR::calcPlayerJointMtx(&jointMtx, "CapPosition");

    TVec3f posOffset;
    jointMtx.mult(::sMarioCapPosOffset, posOffset);

    TVec3f vel = posOffset - mPosition;
    // This can really only be sHiveAccel, but this nerve is only accessible by Wait
    vel.setLength(0.05f);
    mVelocity += vel;

    if (::sGotoSleepingMarioSpeedMax < mVelocity.length()) {
        mVelocity.setLength(::sGotoSleepingMarioSpeedMax);
    }

    if (mPosition.distance(posOffset) < ::sPerchOnNearDistance && MR::checkPassBckFrame(this, 0.0f)) {
        mVelocity.zero();
        setNerve(&NrvButterfly::HostTypeReadyToPerchOnSleepingMario::sInstance);
    }
}

void Butterfly::exeReadyToPerchOnSleepingMario() {
    TPos3f jointMtx;
    MR::calcPlayerJointMtx(&jointMtx, "CapPosition");

    TVec3f posOffset;
    jointMtx.mult(::sMarioCapPosOffset, posOffset);
    MR::vecBlend(mPosition, posOffset, &mPosition, ::sTransBlendRatio);

    MR::setBckRate(this, MR::converge(MR::getBckRate(this), ::sReadyToPerchOnAnimRate, ::sAnimInterpolateRate));
    if (MR::checkPassBckFrame(this, 0.0f)) {
        setNerve(&NrvButterfly::HostTypePerchOnSleepingMario::sInstance);
    }
}

void Butterfly::exePerchOnSleepingMario() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    TPos3f jointMtx;
    MR::calcPlayerJointMtx(&jointMtx, "CapPosition");

    jointMtx.mult(::sMarioCapPosOffset, mPosition);

    if (!MR::isPlayerSleeping()) {
        MR::startBck(this, "Butterfly", nullptr);
        setNerve(&NrvButterfly::HostTypeWait::sInstance);
    }
}
