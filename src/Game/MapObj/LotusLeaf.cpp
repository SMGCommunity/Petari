#include "Game/MapObj/LotusLeaf.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sShadowRadius = 300.0f;
    static const f32 sShakeInitSpeed = 1.5f;
    static const f32 sShakeSpeedAtten = 0.95f;
    static const f32 sShakeSpeedMin = 0.005f;
    static const f32 sShakeAccelMin = 0.0001f;
    static const f32 sAfloatSpeed = 0.1f;
    static const f32 sAfloatAccel = 0.04f;
    static const f32 sShakePeriodStart = 17.0f;
    static const f32 sShakePeriodSlowPitch = 0.65f;
    static const f32 sSinkDepthMax = 25.0f;
};  // namespace

namespace NrvLotusLeaf {
    NEW_NERVE(HostTypeWait, LotusLeaf, Wait);
    NEW_NERVE(HostTypeShakeOnPlayer, LotusLeaf, Shake);
    NEW_NERVE(HostTypeShake, LotusLeaf, Shake);
    NEW_NERVE(HostTypeWaitPlayerOn, LotusLeaf, WaitPlayerOn);
};  // namespace NrvLotusLeaf

LotusLeaf::LotusLeaf(const char* pName) : LiveActor(pName), mInitPos(gZeroVec), mShakeSpeed(), mShakePeriod() {
}

void LotusLeaf::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mInitPos.set(mPosition);
    initModelManagerWithAnm("LotusLeaf", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::initCollisionParts(this, "LotusLeaf", MR::addBodyMessageSensorMapObj(this), nullptr);
    initSound(4, false);
    initNerve(GET_NERVE(LotusLeaf, HostTypeWait));
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostSRT(this, "LotusLeafRipple", &mInitPos, nullptr, nullptr);
    MR::initShadowVolumeCylinder(this, ::sShadowRadius);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void LotusLeaf::exeWait() {
    convergeToInitPos();

    if (MR::isFirstStep(this)) {
        MR::deleteEffect(this, "LotusLeafRipple");
    }

    if (MR::isOnPlayer(this)) {
        setNerve(GET_NERVE(LotusLeaf, HostTypeShakeOnPlayer));
    }
}

void LotusLeaf::exeWaitPlayerOn() {
    if (mInitPos.y < mPosition.y) {
        convergeToInitPos();
    }

    if (MR::isOnPlayer(this)) {
        return;
    }

    setNerve(GET_NERVE(LotusLeaf, HostTypeWait));
}

void LotusLeaf::exeShake() {
    // FIXME: incorrect load time for mVelocity vector register, regswaps
    // https://decomp.me/scratch/oDDPt

    if (MR::isFirstStep(this)) {
        mShakeSpeed = ::sShakeInitSpeed;
        mShakePeriod = ::sShakePeriodStart;

        MR::startSound(this, "SE_OJ_LOTUS_LEAF_WAVE");
        MR::emitEffect(this, "LotusLeafRipple");
    }

    f32 f1 = TWO_PI / mShakePeriod;
    f32 angle = getNerveStep() * f1;
    f32 vel = -mShakeSpeed * MR::cos(angle);
    mShakePeriod += ::sShakePeriodSlowPitch;
    mShakeSpeed *= ::sShakeSpeedAtten;

    TVec3f& rVelocity = mVelocity;
    f32 accel = vel - rVelocity.y;
    rVelocity.y = vel;

    if (mPosition.y + mVelocity.y <= mInitPos.y - ::sSinkDepthMax) {
        rVelocity.zero();
    }

    if (isNerve(GET_NERVE(LotusLeaf, HostTypeShake))) {
        if (MR::isOnPlayer(this)) {
            LiveActor::setNerve(GET_NERVE(LotusLeaf, HostTypeShakeOnPlayer));
            return;
        }
    } else {
        if (!MR::isOnPlayer(this)) {
            setNerve(GET_NERVE(LotusLeaf, HostTypeShake));
            return;
        }
    }

    if (MR::isNearZero(accel, ::sShakeAccelMin) && MR::isNearZero(vel, ::sShakeSpeedMin)) {
        mVelocity.zero();

        if (isNerve(GET_NERVE(LotusLeaf, HostTypeShakeOnPlayer))) {
            setNerve(GET_NERVE(LotusLeaf, HostTypeWaitPlayerOn));
        } else {
            setNerve(GET_NERVE(LotusLeaf, HostTypeWait));
        }
    }
}

void LotusLeaf::convergeToInitPos() {
    if (mPosition.y < mInitPos.y) {
        mVelocity.y += ::sAfloatAccel;
        mVelocity.y = MR::min(mVelocity.y, ::sAfloatSpeed);

        if (mInitPos.y <= mPosition.y + mVelocity.y) {
            mPosition.y = mInitPos.y;
            mVelocity.zero();
        }
    } else {
        mVelocity.y -= ::sAfloatAccel;
        mVelocity.y = MR::max(mVelocity.y, -::sAfloatSpeed);

        if (mPosition.y + mVelocity.y <= mInitPos.y) {
            mPosition.y = mInitPos.y;
            mVelocity.zero();
        }
    }
}
