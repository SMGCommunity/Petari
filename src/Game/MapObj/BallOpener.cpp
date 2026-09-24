#include "Game/MapObj/BallOpener.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void BallOpener_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.000003814697265625f;
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
    const s32 sLaunchFixTime = 20;
    const s32 sSetUpTime = 45;
    const f32 sLaunchValidHeight = 160.0f;
    // sOpenVelocityRateH
    // sOpenVelocityV
}  // namespace

namespace NrvBallOpener {
    NEW_NERVE(BallOpenerNrvWait, BallOpener, Wait);
    NEW_NERVE(BallOpenerNrvSetCenter, BallOpener, SetCenter);
    NEW_NERVE(BallOpenerNrvOpen, BallOpener, Open);
};  // namespace NrvBallOpener

BallOpener::~BallOpener() {
}

BallOpener::BallOpener(const char* pName)
    : LiveActor(pName), mBoundSensor(), _A0(0, 0, 0), mInitialVelocity(0, 0, 0), mBindVelocity(0, 0, 0), mSettledFrames() {
    mCone = new BindCone(mPosition, TVec3f(0.0f, 1.0f, 0.0f), ::sHoleDepth, ::sHoleRadius);
}

void BallOpener::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("BallOpener", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_JUMP_HOLE, 8, ::sHoleRadius, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);
    MR::initCollisionParts(this, "BallOpener", getSensor(nullptr), nullptr);
    mCone->setPosition(mPosition);
    initNerve(GET_NERVE(BallOpener, BallOpenerNrvWait));
    MR::useStageSwitchWriteA(this, rIter);
    appear();
}

void BallOpener::initAfterPlacement() {
    MR::calcGravityVector(this, &mHoleGravity, nullptr, 0);
    mCone->setDirection(mHoleGravity);
}

void BallOpener::control() {
}

bool BallOpener::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SPHERE_PLAYER_BINDED) {
        f32 radiusSquared = pReceiver->mRadius * pReceiver->mRadius;

        TVec3f offset = pReceiver->mPosition - pSender->mPosition;
        TVec3f lateral;
        lateral.killElement2(offset, mHoleGravity);

        if (lateral.squared() <= radiusSquared && offset.squared(lateral) <= ::sLaunchValidHeight * ::sLaunchValidHeight) {
            mBoundSensor = pSender;
            mInitialVelocity = pSender->mHost->mVelocity;
            mBindVelocity = mInitialVelocity;
            MR::zeroVelocity(mBoundSensor->mHost);
            return true;
        }
    }

    return false;
}

void BallOpener::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    if (mBoundSensor != nullptr) {
        bindHole();
        setNerve(GET_NERVE(BallOpener, BallOpenerNrvSetCenter));
    }
}

void BallOpener::exeSetCenter() {
    if (MR::isFirstStep(this)) {
        mSettledFrames = 0;
        MR::invalidateClipping(this);
    }

    bindHole();

    if (mBindVelocity.squared() < 0.0001f) {
        mSettledFrames++;
    } else {
        mSettledFrames = 0;
    }

    if (mSettledFrames == 1) {
        MR::shakeCameraWeak();
    }

    MR::startLevelSound(this, "SE_OJ_LV_BALL_OPN_SETTING");

    if (mSettledFrames > ::sLaunchFixTime || MR::isGreaterStep(this, ::sForceSetCenterTime)) {
        setNerve(GET_NERVE(BallOpener, BallOpenerNrvOpen));
    }
}

void BallOpener::exeOpen() {
    if (MR::isFirstStep(this)) {
        mBoundSensor->receiveMessage(ACTMES_SET_UP_JUMP_HOLE, getSensor("body"));
    }

    if (MR::isStep(this, ::sSetUpTime)) {
        MR::zeroVelocity(mBoundSensor->mHost);
        mBoundSensor->receiveMessage(ACTMES_END_BALL_BIND, getSensor("body"));
        mBoundSensor = nullptr;
        MR::startSound(this, "SE_OJ_BALL_OPN_OPEN");
        MR::startSystemSE("SE_SY_TAMAKORO_GOAL");

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        setNerve(GET_NERVE(BallOpener, BallOpenerNrvWait));
    }
}

void BallOpener::bindHole() {
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
