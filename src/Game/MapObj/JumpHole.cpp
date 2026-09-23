#include "Game/MapObj/JumpHole.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void JumpHole_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.000003814697265625f;
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
    (void)2.0f;
}

namespace {
    const f32 sGravityAcc = 1.2f;
    const f32 sHoleRadius = 200.0f;
    const f32 sHoleDepth = 400.0f;
    const f32 sHoleRefrecRate = 0.9f;
    const s32 sForceSetStartTime = 40;
    const s32 sForceSetEndTime = 90;
    const f32 sBallToCenterFreq = 0.98f;
    const f32 sBallToCenterEndFreq = 0.7f;
    const s32 sForceSetCenterTime = 180;
    // sLaunchVelocity
    const s32 sLaunchFixTime = 10;
    const s32 sSetUpTime = 45;
    const f32 sLaunchValidHeight = 160.0f;
    // sVelocityScale
    // sTestBindSphereRadius

    static TVec3f sTestBindSpherePos(0, 0, 0);
    static TVec3f sTestBindSphereVelocity(0, 0, 0);
};  // namespace

namespace NrvJumpHole {
    NEW_NERVE(JumpHoleNrvWait, JumpHole, Wait);
    NEW_NERVE(JumpHoleNrvSetCenter, JumpHole, SetCenter);
    NEW_NERVE(JumpHoleNrvSetUp, JumpHole, SetUp);
    NEW_NERVE(JumpHoleNrvRailMove, JumpHole, RailMove);
};  // namespace NrvJumpHole

JumpHole::JumpHole(const char* pName)
    : LiveActor(pName), mBoundSensor(), mLaunchStart(0, 0, 0), mBindVelocity(0, 0, 0), mHoleGravity(0, 0, 0), mSettledFrames() {
    mCone = new BindCone(mPosition, TVec3f(0.0f, 1.0f, 0.0f), ::sHoleDepth, ::sHoleRadius);
}

void JumpHole::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("JumpHole", nullptr, false);
    MR::connectToSceneMapObj(this);
    initRailRider(rIter);
    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_JUMP_HOLE, 8, ::sHoleRadius, TVec3f(0.0f, 0.0f, 0.0f));
    initSound(4, false);
    MR::initCollisionParts(this, "JumpHole", getSensor(nullptr), nullptr);
    mCone->setPosition(mPosition);
    initNerve(GET_NERVE(JumpHole, JumpHoleNrvWait));

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void JumpHole::initAfterPlacement() {
    MR::calcGravityVector(this, &mHoleGravity, nullptr, 0);
    mCone->setDirection(mHoleGravity);
}

void JumpHole::control() {
    return;
}

bool JumpHole::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SPHERE_PLAYER_BINDED) {
        f32 radiusSquared = pReceiver->mRadius * pReceiver->mRadius;
        TVec3f offset = pReceiver->mPosition - pSender->mPosition;
        TVec3f lateral;
        lateral.killElement2(offset, mHoleGravity);

        if (lateral.squared() <= radiusSquared && offset.squared(lateral) <= ::sLaunchValidHeight * ::sLaunchValidHeight) {
            mBoundSensor = pSender;
            mBindVelocity = pSender->mHost->mVelocity;
            MR::zeroVelocity(mBoundSensor->mHost);
            return true;
        }
    }

    return false;
}

void JumpHole::exeWait() {
    if (mBoundSensor != nullptr) {
        bindHole();
        setNerve(GET_NERVE(JumpHole, JumpHoleNrvSetCenter));
        MR::invalidateClipping(this);
    }
}

void JumpHole::exeSetCenter() {
    if (MR::isFirstStep(this)) {
        mSettledFrames = 0;
    }

    bindHole();

    if (mBindVelocity.squared() < 0.0001f) {
        mSettledFrames++;
    } else {
        mSettledFrames = 0;
    }

    MR::startLevelSound(this, "SE_OJ_LV_JUMP_HOLE_SETTING");

    if (mSettledFrames > ::sLaunchFixTime || MR::isGreaterStep(this, ::sForceSetCenterTime)) {
        setNerve(GET_NERVE(JumpHole, JumpHoleNrvSetUp));
        mBindVelocity.zero();
    }
}

void JumpHole::exeSetUp() {
    if (MR::isFirstStep(this)) {
        mBoundSensor->receiveMessage(ACTMES_SET_UP_JUMP_HOLE, getSensor("body"));
    }

    if (MR::isGreaterStep(this, ::sSetUpTime)) {
        setNerve(GET_NERVE(JumpHole, JumpHoleNrvRailMove));
    }
}

void JumpHole::exeRailMove() {
    if (MR::isFirstStep(this)) {
        mLaunchTime = 0.0f;
        initParabola(mBoundSensor->mHost->mPosition);
        MR::shakeCameraNormal();
        MR::startSound(this, "SE_OJ_JUMP_HOLE_FLIP");
        MR::startSoundPlayer("SE_PV_JUMP_JOY", -1);
        MR::startSoundPlayer("SE_PM_JUMP_LONG", -1);
        mBoundSensor->receiveMessage(ACTMES_SHOOT_JUMP_HOLE, getSensor("body"));
    }

    f32 horizontal = mLaunchDistance * mLaunchTime;
    f32 vertical = mLaunchTime * (mLaunchLinear + mLaunchQuadratic * mLaunchTime);
    TVec3f position = mLaunchStart + mLaunchGravity * vertical + mLaunchDirection * horizontal;
    TVec3f previous(mBoundSensor->mHost->mPosition);
    mBoundSensor->mHost->mPosition.set(position);
    mLaunchTime += 1.0f / mLaunchDuration;

    if (mLaunchTime >= 1.0f) {
        mBoundSensor->mHost->mVelocity.set(position - previous);
        mBoundSensor->receiveMessage(ACTMES_END_JUMP_HOLE, getSensor("body"));
        mBoundSensor = nullptr;
        setNerve(GET_NERVE(JumpHole, JumpHoleNrvWait));
        MR::validateClipping(this);
    }
}

void JumpHole::bindHole() {
    f32 radius = mBoundSensor->mRadius;
    TVec3f previous(mBoundSensor->mPosition);
    TVec3f position(previous);
    mBindVelocity += mHoleGravity * ::sGravityAcc;

    BindResult result;
    BindSphere sphere(previous, radius);
    MR::bindSpereToCone(&result, mBindVelocity, sphere, *mCone);
    MR::updateBindPositionAndVelocity(&position, &mBindVelocity, result, ::sHoleRefrecRate);

    TVec3f lateralVelocity;
    lateralVelocity.killElement2(mBindVelocity, mHoleGravity);
    TVec3f lateralOffset;
    lateralOffset.killElement2(position - mPosition, mHoleGravity);
    f32 limit = ::sHoleRadius - radius;
    if (lateralVelocity.dot(lateralOffset) > 0.0f && lateralOffset.squared() > limit * limit) {
        f32 distance;
        MR::separateScalarAndDirection(&distance, &lateralOffset, lateralOffset);
        position -= lateralOffset * (distance - limit);

        TVec3f tangent = lateralVelocity - lateralOffset * lateralVelocity.dot(lateralOffset);
        if (tangent.isZero()) {
            tangent = mHoleGravity;
        } else {
            MR::normalizeOrZero(&tangent);
        }

        mBindVelocity -= lateralVelocity;
        mBindVelocity += tangent * lateralVelocity.length();
    }

    mBindVelocity *= MR::calcNerveValue(this, ::sForceSetStartTime, ::sForceSetEndTime, ::sBallToCenterFreq, ::sBallToCenterEndFreq);
    mBoundSensor->mHost->mVelocity.set(position - previous);

    if (position.squared(previous) < 0.1f * 0.1f) {
        mBindVelocity.zero();
    }
}

void JumpHole::initParabola(const TVec3f& rPosition) {
    MR::moveCoordToStartPos(this);
    TVec3f start(MR::getRailPos(this));
    MR::moveCoordToEndPos(this);
    TVec3f end(MR::getRailPos(this));
    MR::calcGravityVector(this, end, &mLaunchGravity, nullptr, 0);

    if (MR::isNearZero(mLaunchGravity)) {
        mLaunchGravity.set(0.0f, -1.0f, 0.0f);
    }

    f32 startHeight = mLaunchGravity.dot(start - rPosition);
    f32 endHeight = mLaunchGravity.dot(end - rPosition);
    mLaunchDirection = end - rPosition - mLaunchGravity * endHeight;
    MR::separateScalarAndDirection(&mLaunchDistance, &mLaunchDirection, mLaunchDirection);
    MR::calcParabolicFunctionParam(&mLaunchQuadratic, &mLaunchLinear, startHeight, endHeight);
    mLaunchStart = rPosition;
    mLaunchDuration = MR::fastSqrtf(MR::abs(2.0f * mLaunchQuadratic / ::sGravityAcc));
}
