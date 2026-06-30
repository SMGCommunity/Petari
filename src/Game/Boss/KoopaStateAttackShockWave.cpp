#include "Game/Boss/KoopaStateAttackShockWave.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sJumpSpeed1st = 50.0f;
    static const f32 sJumpSpeed = 40.0f;
    static const f32 sJumpSpeedVs3 = 40.0f;
    static const f32 sJumpGravity = 1.5f;
    static const f32 sJumpGravityVs3 = 2.0f;
    static const f32 sJumpTurnSpeedVs3 = 3.0f;
    static const s32 sLandStep = 15;
    static const s32 sLandStepVs2 = 10;
    static const s32 sLandStepVs3 = 10;
    static const s32 sLandStepVs3Angry = 5;
};  // namespace

namespace NrvKoopaStateAttackShockWave {
    NEW_NERVE(KoopaStateAttackShockWaveNrvJumpStart, KoopaStateAttackShockWave, JumpStart);
    NEW_NERVE(KoopaStateAttackShockWaveNrvJump, KoopaStateAttackShockWave, Jump);
    NEW_NERVE(KoopaStateAttackShockWaveNrvJumpFast, KoopaStateAttackShockWave, Jump);
    NEW_NERVE(KoopaStateAttackShockWaveNrvLand, KoopaStateAttackShockWave, Land);
};  // namespace NrvKoopaStateAttackShockWave

KoopaStateAttackShockWave::KoopaStateAttackShockWave(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[衝撃波攻撃]", pKoopa), mMaxAttacks(1), mAttacks(), mJumpVelocity(::sJumpSpeed), mGravity(::sJumpGravity),
      mJumpDelay(::sLandStep) {
}

void KoopaStateAttackShockWave::init() {
    initNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpStart::sInstance);
    kill();
}

void KoopaStateAttackShockWave::appear() {
    mIsDead = false;
    mAttacks = 0;

    MR::validateHitSensor(mHost, "AttackHipDrop");

    if (KoopaFunction::isKoopaVs1(mHost)) {
        if (KoopaFunction::isKoopaLv3(mHost)) {
            mMaxAttacks = 3;
        } else if (KoopaFunction::isKoopaLv2(mHost)) {
            mMaxAttacks = 2;
        } else {
            mMaxAttacks = 1;
        }

        mJumpVelocity = ::sJumpSpeed;
        mGravity = ::sJumpGravity;
        mJumpDelay = ::sLandStep;
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        if (KoopaFunction::isKoopaLv3(mHost)) {
            mMaxAttacks = 5;
        } else {
            mMaxAttacks = 3;
        }

        mJumpVelocity = ::sJumpSpeed;
        mGravity = ::sJumpGravity;
        mJumpDelay = ::sLandStepVs2;
    } else {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            mMaxAttacks = 3;

            if (KoopaFunction::isKoopaAngry(mHost)) {
                mJumpVelocity = ::sJumpSpeed;
                mGravity = ::sJumpGravity;
                mJumpDelay = ::sLandStepVs3Angry;
            } else {
                mJumpVelocity = ::sJumpSpeedVs3;
                mGravity = ::sJumpGravityVs3;
                mJumpDelay = ::sLandStepVs2;
            }
        } else {
            mJumpVelocity = ::sJumpSpeedVs3;
            mGravity = ::sJumpGravityVs3;

            if (KoopaFunction::isKoopaAngry(mHost)) {
                mMaxAttacks = 5;
                mJumpDelay = ::sLandStepVs3Angry;
            } else {
                mMaxAttacks = 3;
                mJumpDelay = ::sLandStepVs3;
            }
        }
    }

    setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpStart::sInstance);
}

void KoopaStateAttackShockWave::kill() {
    MR::invalidateHitSensor(mHost, "AttackHipDrop");

    mIsDead = true;
}

bool KoopaStateAttackShockWave::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if ((isNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJump::sInstance) ||
         isNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpFast::sInstance)) &&
        MR::isSensor(pSender, "AttackHipDrop") && MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
        return true;
    }

    if (KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver)) {
        return true;
    }

    if (KoopaFunction::tryKoopaBodyAttackPlayer(pSender, pReceiver)) {
        return true;
    }

    return KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver);
}

void KoopaStateAttackShockWave::exeJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "ShockWaveJumpStart");
    }

    if (MR::isBckStopped(mHost)) {
        setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJump::sInstance);
    }
}

void KoopaStateAttackShockWave::exeJump() {
    if (MR::isFirstStep(this)) {
        if (mAttacks == mMaxAttacks - 1) {
            MR::startAction(mHost, "ShockWaveJump3rd");
        } else if (isNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpFast::sInstance)) {
            MR::startAction(mHost, "JumpFast");
        } else {
            MR::startAction(mHost, "ShockWaveJump");
        }

        MR::startSound(mHost, "SE_BM_KOOPA_JUMP");

        if (mAttacks == 0) {
            MR::setVelocityJump(mHost, ::sJumpSpeed1st);
        } else {
            MR::setVelocityJump(mHost, mJumpVelocity);
        }
    }

    MR::addVelocityToGravity(mHost, mGravity);

    if (KoopaFunction::isKoopaVs3(mHost) && KoopaFunction::isKoopaLv3(mHost)) {
        Koopa* pKoopa = mHost;
        MR::turnDirectionToPlayerDegree(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), ::sJumpTurnSpeedVs3);
    }

    Koopa* pKoopa = mHost;
    if (!MR::sendMsgEnemyAttackToBindedSensor(pKoopa, pKoopa->getSensor("Body")) && MR::isOnGround(mHost)) {
        MR::tryRumblePadAndCameraDistanceMiddle(mHost, 1500.0f, 2500.0f, 2000.0f);
        MR::shakeCameraWeak();

        MR::startSound(mHost, "SE_BM_KOOPA_LAND");

        KoopaFunction::emitKoopaShockWave(mHost);

        mHost->mVelocity.set2(0.0f);

        setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvLand::sInstance);
    }
}

void KoopaStateAttackShockWave::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "ShockWaveLand");
    }

    if (MR::isStep(this, mJumpDelay) && ++mAttacks < mMaxAttacks) {
        setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpFast::sInstance);
    } else if (MR::isActionEnd(mHost)) {
        kill();
    }
}

KoopaStateAttackShockWave::~KoopaStateAttackShockWave() {
}
