#include "Game/MapObj/LotusLeaf.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <math_types.hpp>
#include <revolution/types.h>

namespace {
    static const f32 sShadowRadius = 300.0f;
    static const f32 sShakeInitSpeed = 1.5f;
    static const f32 sShakeSpeedAtten = 0.95f;
    static const f32 sShakeSpeedMin = 0.005f;
    static const f32 sShakeAccelMin = 0.0001f;
    // static const f32 sAfloatSpeed =
    // static const f32 sAfloatAccel =
    static const f32 sShakePeriodStart = 17.0f;
    static const f32 sShakePeriodSlowPitch = 0.65f;
    static const f32 sSinkDepthMax = 25.0f;
}  // namespace

namespace NrvLotusLeaf {
    NEW_NERVE(HostTypeWait, LotusLeaf, Wait);
    NEW_NERVE(HostTypeShakeOnPlayer, LotusLeaf, Shake);
    NEW_NERVE(HostTypeShake, LotusLeaf, Shake);
    NEW_NERVE(HostTypeWaitPlayerOn, LotusLeaf, WaitPlayerOn);
}  // namespace NrvLotusLeaf

LotusLeaf::LotusLeaf(const char* pName) : LiveActor(pName), mInitPos(gZeroVec), mShakeSpeed(0.0f), mShakePeriod(0.0f) {}

void LotusLeaf::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mInitPos.set(mPosition);
    initModelManagerWithAnm("LotusLeaf", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::initCollisionParts(this, "LotusLeaf", MR::addBodyMessageSensorMapObj(this), nullptr);
    initSound(4, false);
    initNerve(&NrvLotusLeaf::HostTypeWait::sInstance);
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostSRT(this, "LotusLeafRipple", &mInitPos, nullptr, nullptr);
    MR::initShadowVolumeCylinder(this, sShadowRadius);
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
    if (MR::isOnPlayer(this))
        setNerve(&NrvLotusLeaf::HostTypeShakeOnPlayer::sInstance);
}

void LotusLeaf::exeWaitPlayerOn() {
    if (mInitPos.y < mPosition.y) {
        convergeToInitPos();
    }
    if (!MR::isOnPlayer(this)) {
        setNerve(&NrvLotusLeaf::HostTypeWait::sInstance);
    }
}

void LotusLeaf::exeShake() {
    // FIXME: incorrect load time for mVelocity vector register, regswaps
    // https://decomp.me/scratch/oDDPt

    if (MR::isFirstStep(this)) {
        mShakeSpeed = sShakeInitSpeed;
        mShakePeriod = sShakePeriodStart;
        MR::startSound(this, "SE_OJ_LOTUS_LEAF_WAVE", -1, -1);
        MR::emitEffect(this, "LotusLeafRipple");
    }

    f32 f1 = TWO_PI / mShakePeriod;
    f32 vel = -mShakeSpeed * JMath::sSinCosTable.cosLapRad(getNerveStep() * f1);
    mShakePeriod += sShakePeriodSlowPitch;
    mShakeSpeed *= sShakeSpeedAtten;

    f32 accel = vel - mVelocity.y;
    mVelocity.y = vel;

    if (mPosition.y + mVelocity.y <= mInitPos.y - sSinkDepthMax) {
        mVelocity.zero();
    }

    if (isNerve(&NrvLotusLeaf::HostTypeShake::sInstance)) {
        if (MR::isOnPlayer(this)) {
            LiveActor::setNerve(&NrvLotusLeaf::HostTypeShakeOnPlayer::sInstance);
            return;
        }
    } else {
        if (!MR::isOnPlayer(this)) {
            setNerve(&NrvLotusLeaf::HostTypeShake::sInstance);
            return;
        }
    }

    if (MR::isNearZero(accel, sShakeAccelMin) && MR::isNearZero(vel, sShakeSpeedMin)) {
        mVelocity.zero();
        if (isNerve(&NrvLotusLeaf::HostTypeShakeOnPlayer::sInstance)) {
            setNerve(&NrvLotusLeaf::HostTypeWaitPlayerOn::sInstance);
        } else {
            setNerve(&NrvLotusLeaf::HostTypeWait::sInstance);
        }
    }
}

void LotusLeaf::convergeToInitPos() {
    if (mPosition.y < mInitPos.y) {
        mVelocity.y += 0.04f;
        mVelocity.y = (mVelocity.y >= 0.1f) ? 0.1f : mVelocity.y;

        if (mInitPos.y <= mPosition.y + mVelocity.y) {
            mPosition.y = mInitPos.y;
            mVelocity.zero();
        }
    } else {
        mVelocity.y -= 0.04f;
        mVelocity.y = (mVelocity.y >= -0.1f) ? mVelocity.y : -0.1f;

        if (mPosition.y + mVelocity.y <= mInitPos.y) {
            mPosition.y = mInitPos.y;
            mVelocity.zero();
        }
    }
}
