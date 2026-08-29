#include "Game/MapObj/DragonHeadFlower.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"

namespace {
    const f32 sGravityAcc = 0.94f;
    const f32 sBreatheAccel = 3.0f;
    const s32 sBreatheFreq = 40;
    const f32 sStartLaunchRange = 50.0f;
    const f32 sStartForceFixPos = 0.0f;
    const f32 sEndForceFixPos = 1.0f;
    const s32 sDelayStartToCenter = 15;
};  // namespace

namespace NrvDragonHeadFlower {
    NEW_NERVE(DragonHeadFlowerNrvWait, DragonHeadFlower, Wait);
    NEW_NERVE(DragonHeadFlowerNrvSetCenter, DragonHeadFlower, SetCenter);
    NEW_NERVE(DragonHeadFlowerNrvLaunchReady, DragonHeadFlower, LaunchReady);
    NEW_NERVE(DragonHeadFlowerNrvRailMove, DragonHeadFlower, RailMove);
};  // namespace NrvDragonHeadFlower

DragonHeadFlower::DragonHeadFlower(const char* pName)
    : LiveActor(pName), mPlayerSensor(), mParabolaProgress(), mParabolaStartPos(0, 0, 0), mTargetVelocity(0, 0, 0), mCurrentGravity(0, -1, 0),
      mEndGravity(0, 0, 0), mParabolaDirection(0, 0, 0), mParabolaSteps(), mParabolaDistance(), mParabolaAccel(), mParabolaVel() {
}

void DragonHeadFlower::init(const JMapInfoIter& rIter) {
    const char* pModelName;
    if (!MR::getObjectName(&pModelName, rIter)) {
        pModelName = "DragonHeadFlower";
    }

    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(pModelName, nullptr, false);

    MR::connectToSceneMapObj(this);

    initRailRider(rIter);

    initHitSensor(2);
    MR::addHitSensor(this, "body", ATYPE_JUMP_HOLE, 8, 560.0f, TVec3f(0.0f, 300.0f, 650.0f));
    MR::addHitSensorMapObj(this, "push", 16, 400.0f, TVec3f(0.0f, 350.0f, 0.0f));

    initEffectKeeper(2, nullptr, false);
    initSound(4, false);
    initNerve(GET_NERVE(DragonHeadFlower, DragonHeadFlowerNrvWait));

    appear();

    MR::startBck(this, "Wait", nullptr);
}

void DragonHeadFlower::control() {
}

void DragonHeadFlower::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    MR::sendMsgPush(pReceiver, pSender);
}

bool DragonHeadFlower::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SPHERE_PLAYER_BINDED) {
        mPlayerSensor = pReceiver;
        mTargetVelocity = pReceiver->mHost->mVelocity;

        MR::zeroVelocity(mPlayerSensor->mHost);

        return true;
    }

    return false;
}

void DragonHeadFlower::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckWithInterpole(this, "Wait", ::sDelayStartToCenter);
        MR::validateClipping(this);
    }

    if (mPlayerSensor != nullptr) {
        MR::calcGravityVector(this, mPosition, &mCurrentGravity, nullptr, 0);
        setNerve(GET_NERVE(DragonHeadFlower, DragonHeadFlowerNrvSetCenter));
    }
}

void DragonHeadFlower::exeSetCenter() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Breathe", nullptr);
        MR::invalidateClipping(this);
    }

    if (getNerveStep() <= 0) {
        return;
    }

    TVec3f targetPos(mPlayerSensor->mPosition);

    TVec3f launchPos;
    MR::copyJointPos(this, "Point", &launchPos);

    targetPos += mTargetVelocity;

    f32 t = MR::clamp(static_cast< f32 >(getNerveStep() - ::sBreatheFreq) / ::sBreatheFreq, ::sStartForceFixPos, ::sEndForceFixPos);

    TVec3f lerpLaunch(launchPos);
    lerpLaunch.x *= t;
    lerpLaunch.y *= t;
    lerpLaunch.z *= t;

    TVec3f lerpTarget(targetPos);
    lerpTarget *= 1.0f - t;

    targetPos = lerpTarget + lerpLaunch;

    mPlayerSensor->mHost->mPosition.set(targetPos + (mPlayerSensor->mHost->mPosition - mPlayerSensor->mPosition));

    TVec3f targetDir(launchPos - targetPos);

    f32 targetVel;
    MR::separateScalarAndDirection(&targetVel, &targetDir, targetDir);

    mTargetVelocity += targetDir * ::sBreatheAccel;
    mTargetVelocity.x *= ::sGravityAcc;
    mTargetVelocity.y *= ::sGravityAcc;
    mTargetVelocity.z *= ::sGravityAcc;

    if (targetVel < ::sStartLaunchRange) {
        setNerve(GET_NERVE(DragonHeadFlower, DragonHeadFlowerNrvLaunchReady));
    }
}

void DragonHeadFlower::exeLaunchReady() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "In", nullptr);

        MR::hidePlayer();
        MR::hideModel(mPlayerSensor->mHost);
    }

    TVec3f launchPos;
    MR::copyJointPos(this, "Point", &launchPos);
    mPlayerSensor->mHost->mPosition.set(launchPos + (mPlayerSensor->mHost->mPosition - mPlayerSensor->mPosition));

    if (!MR::isBckStopped(this)) {
        return;
    }

    mParabolaProgress = 0.0f;

    initParabola(mPlayerSensor->mHost->mPosition);

    setNerve(GET_NERVE(DragonHeadFlower, DragonHeadFlowerNrvRailMove));
}

void DragonHeadFlower::exeRailMove() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Out", nullptr);

        MR::shakeCameraNormal();

        MR::emitEffect(this, "Launch");

        MR::startSoundPlayer("SE_PV_JUMP_JOY", -1);

        MR::showPlayer();
        MR::showModel(mPlayerSensor->mHost);
    }

    f32 f1 = mParabolaDistance * mParabolaProgress;
    f32 f2 = mParabolaProgress * (mParabolaVel + mParabolaAccel * mParabolaProgress);

    TVec3f targetPos(mParabolaStartPos + mEndGravity * f2 + mParabolaDirection * f1);
    TVec3f prevPos(mPlayerSensor->mHost->mPosition);

    mPlayerSensor->mHost->mPosition.set(targetPos);

    mParabolaProgress += 1.0f / mParabolaSteps;

    if (mParabolaProgress >= 1.0f) {
        TVec3f* velocity = &mPlayerSensor->mHost->mVelocity;
        velocity->set(targetPos - prevPos);
        mPlayerSensor->receiveMessage(ACTMES_END_JUMP_HOLE, getSensor("body"));

        mPlayerSensor = nullptr;

        setNerve(GET_NERVE(DragonHeadFlower, DragonHeadFlowerNrvWait));
    }
}

void DragonHeadFlower::initParabola(const TVec3f& rStartPos) {
    MR::moveCoordToStartPos(this);
    TVec3f railStartPos(MR::getRailPos(this));

    MR::moveCoordToEndPos(this);
    TVec3f railEndPos(MR::getRailPos(this));

    MR::calcGravityVector(this, railEndPos, &mEndGravity, nullptr, 0);

    if (MR::isNearZero(mEndGravity)) {
        mEndGravity.set(0.0f, -1.0f, 0.0f);
    }

    f32 max = mEndGravity.dot(railStartPos - rStartPos);
    f32 end = mEndGravity.dot(railEndPos - rStartPos);
    mParabolaDirection = railEndPos - rStartPos - mEndGravity * end;

    MR::separateScalarAndDirection(&mParabolaDistance, &mParabolaDirection, mParabolaDirection);
    MR::calcParabolicFunctionParam(&mParabolaAccel, &mParabolaVel, max, end);

    mParabolaStartPos = rStartPos;

    mParabolaSteps = MR::sqrt(MR::abs((mParabolaAccel * 2.0f) / 1.2f));
}
