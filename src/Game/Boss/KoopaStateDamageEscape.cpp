#include "Game/Boss/KoopaStateDamageEscape.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static MR::ActorMoveParam sEscapeStartParam = {1.8f, 1.5f, 0.95f, 3.0f};
    static MR::ActorMoveParam sEscapeRunParamLv1 = {7.0f, 1.5f, 0.8f, 0.0f};
    static MR::ActorMoveParam sEscapeRunParamLv2 = {7.0f, 1.5f, 0.8f, 0.2f};
    static MR::ActorMoveParam sEscapeRunParamLv3 = {7.0f, 1.5f, 0.8f, 0.4f};
    static MR::ActorSightParam sEscapeFindSight = {1500.0f, 60.0f, 30.0f};
    static MR::ActorMoveParam sEscapeFindParam = {0.0f, 1.0f, 0.9f, 0.0f};
    static MR::ActorMoveParam sEscapeEndParam = {0.0f, 1.0f, 0.95f, 0.0f};
    static MR::ActorMoveParam sDamageTailRunParamVs1Lv1 = {0.8f, 1.5f, 0.8f, 0.0f};
    static MR::ActorMoveParam sDamageTailRunParam = {6.0f, 1.5f, 0.8f, 0.0f};
    static MR::ActorMoveParam sDamageTailRunParamLv3 = {8.0f, 1.5f, 0.8f, 0.0f};
    static MR::ActorMoveParam sDamageTailRunFinalParam = {13.0f, 3.0f, 0.8f, 0.0f};
    static MR::ActorMoveParam sDamageTailRunEndParam = {0.0f, 1.0f, 0.98f, 0.0f};
    static const f32 sEscapeStartJumpSpeed = 80.0f;
    // static const s32 sEscapeStartStepToValid = _;
    // static const s32 sEscapeStopSceneStep = _;
    static const s32 sEscapeStepVs1Lv1 = 900;
    static const s32 sEscapeStep = 600;
    static const f32 sEscapeRunWallJumpSpeed = 15.0f;
    static const s32 sDamageTailStep = 600;
    static const s32 sDamageTailStepFinal = 480;
    static const s32 sDamageTailRunStartBreakStep = 40;
    static const f32 sDamageTailRunStartSpeed = 40.0f;
    static const f32 sDamageTailWallJumpSpeed = 15.0f;
    static const s32 sDamageTailRunEndRecoverStep = 105;
    static const f32 sDamageTailTurnDegree = 0.1f;
    // static const s32 sDamageTailStopSceneStep = _;
    // static const s32 sDamageTailStepToInvalid = _;
    // static const s32 sDownStep = _;
    // static const s32 sDownStepVs3Angry = _;
    static const s32 sDownStopSceneStep = 5;
    // static const f32 sDownFrontSpeed = _;
    static const f32 sDownFlyUpSpeed = 40.0f;
    static const f32 sDownFlyUpSpeedLv3 = 60.0f;
    static const f32 sDownGravity = 2.0f;
    // static const f32 sJumpFrontSpeed = _;
    // static const f32 sJumpFlyUpSpeed = _;
    // static const f32 sJumpGravity = _;
    // static const s32 sLandStepVs3Lv3 = _;
};  // namespace

namespace NrvKoopaStateDamageEscape {
    NEW_NERVE(KoopaStateDamageEscapeNrvEscapeStart, KoopaStateDamageEscape, EscapeStart);
    NEW_NERVE(KoopaStateDamageEscapeNrvEscapeStartLoop, KoopaStateDamageEscape, EscapeStartLoop);
    NEW_NERVE(KoopaStateDamageEscapeNrvEscapeStartLand, KoopaStateDamageEscape, EscapeStartLand);
    NEW_NERVE(KoopaStateDamageEscapeNrvEscapeRun, KoopaStateDamageEscape, EscapeRun);
    NEW_NERVE(KoopaStateDamageEscapeNrvEscapeFind, KoopaStateDamageEscape, EscapeFind);
    NEW_NERVE(KoopaStateDamageEscapeNrvEscapeEnd, KoopaStateDamageEscape, EscapeEnd);
    NEW_NERVE(KoopaStateDamageEscapeNrvDamageTailRunStart, KoopaStateDamageEscape, DamageTailRunStart);
    NEW_NERVE(KoopaStateDamageEscapeNrvDamageTailRunStartFinal, KoopaStateDamageEscape, DamageTailRunStart);
    NEW_NERVE(KoopaStateDamageEscapeNrvDamageTailRun, KoopaStateDamageEscape, DamageTailRun);
    NEW_NERVE(KoopaStateDamageEscapeNrvDamageTailRunFinal, KoopaStateDamageEscape, DamageTailRun);
    NEW_NERVE(KoopaStateDamageEscapeNrvDamageTailRunEnd, KoopaStateDamageEscape, DamageTailRunEnd);
    NEW_NERVE(KoopaStateDamageEscapeNrvDamageTailRunEndFinal, KoopaStateDamageEscape, DamageTailRunEnd);
    NEW_NERVE(KoopaStateDamageEscapeNrvDown, KoopaStateDamageEscape, Down);
    NEW_NERVE(KoopaStateDamageEscapeNrvDownLand, KoopaStateDamageEscape, DownLand);
    NEW_NERVE(KoopaStateDamageEscapeNrvDownEnd, KoopaStateDamageEscape, DownEnd);
};  // namespace NrvKoopaStateDamageEscape

KoopaStateDamageEscape::KoopaStateDamageEscape(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[逃走]", pKoopa), mEscapeTime(-1), mMaxEscapeTime(-1), _18(-1), mIsTurnClockwise(), mIsLastHit(),
      mEscapeRunParam(&::sEscapeRunParamLv1), mDamageTailRunParam(&::sDamageTailRunParam), mMaxRunFrames(::sDamageTailStep),
      mRotateVelocity(::sDamageTailRunStartSpeed), mJumpAwayVelocity(::sDownFlyUpSpeed) {
}

void KoopaStateDamageEscape::init() {
    initNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeStart::sInstance);

    KoopaFunction::initKoopaCamera(mHost, "逃走開始");
    KoopaFunction::initKoopaCamera(mHost, "逃走");
    KoopaFunction::initKoopaCamera(mHost, "逃走（尻尾ダメージ）");
    KoopaFunction::initKoopaCamera(mHost, "逃走ダウン");

    kill();
}

void KoopaStateDamageEscape::appear() {
    mIsDead = false;

    KoopaFunction::endKoopaCamera(mHost, "逃走開始", false, -1);
    MR::validateHitSensor(mHost, "ReceiverTail");
    MR::validateHitSensor(mHost, "ReceiverTailTop");

    mEscapeTime = 0;
    mMaxRunFrames = ::sDamageTailStep;

    KoopaFunction::changeBgmStateEscape();

    if (KoopaFunction::isKoopaLv1(mHost)) {
        mEscapeRunParam = &::sEscapeRunParamLv1;
        mRotateVelocity = 0.0f;

        if (KoopaFunction::isKoopaVs1(mHost)) {
            mDamageTailRunParam = &::sDamageTailRunParamVs1Lv1;
            mMaxEscapeTime = ::sEscapeStepVs1Lv1;
        } else {
            mMaxEscapeTime = ::sEscapeStep;
            mDamageTailRunParam = &::sDamageTailRunParamLv3;
        }

        mJumpAwayVelocity = ::sDownFlyUpSpeed;
    } else if (KoopaFunction::isKoopaLv2(mHost)) {
        mMaxEscapeTime = ::sEscapeStep;
        mEscapeRunParam = &sEscapeRunParamLv2;
        mRotateVelocity = ::sDamageTailRunStartSpeed;
        mDamageTailRunParam = &::sDamageTailRunParam;
        mJumpAwayVelocity = ::sDownFlyUpSpeed;
    } else {
        mMaxEscapeTime = ::sEscapeStep;
        mEscapeRunParam = &::sEscapeRunParamLv3;
        mRotateVelocity = ::sDamageTailRunStartSpeed;
        mDamageTailRunParam = &::sDamageTailRunParamLv3;
        mJumpAwayVelocity = ::sDownFlyUpSpeedLv3;
    }

    if (KoopaFunction::isKoopaVs1(mHost) || KoopaFunction::isKoopaVs2(mHost)) {
        if (KoopaFunction::isKoopaLv3(mHost)) {
            mIsLastHit = true;
        } else {
            mIsLastHit = false;
        }
    } else {
        if (KoopaFunction::isKoopaAngry(mHost)) {
            mIsLastHit = true;
        } else {
            mIsLastHit = false;
        }
    }

    setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeStart::sInstance);
}

void KoopaStateDamageEscape::kill() {
    KoopaFunction::changeBgmStateNormal(120);

    mIsDead = true;

    KoopaFunction::endKoopaCamera(mHost, "逃走", false, -1);
    KoopaFunction::endKoopaCamera(mHost, "逃走（尻尾ダメージ）", false, -1);
    KoopaFunction::endKoopaCamera(mHost, "逃走ダウン", false, -1);

    MR::invalidateHitSensor(mHost, "ReceiverTail");
    MR::invalidateHitSensor(mHost, "ReceiverTailTop");
    MR::invalidateHitSensor(mHost, "DamageEscapeBody");
    MR::invalidateHitSensor(mHost, "DamageEscapePush");
}

void KoopaStateDamageEscape::startDamageReverse() {
    KoopaFunction::startKoopaSpinHitBlow(mHost);
    KoopaFunction::changeBgmStateDamageTail();
    setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunStart::sInstance);
}

bool KoopaStateDamageEscape::isDownEnd() const {
    if ((KoopaFunction::isKoopaVs1(mHost) || KoopaFunction::isKoopaVs2(mHost)) && KoopaFunction::isKoopaLv3(mHost) &&
        isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDown::sInstance) && MR::isGreaterStep(this, 45)) {
        return true;
    }

    if (KoopaFunction::isKoopaVs3(mHost) && KoopaFunction::isKoopaAngry(mHost) &&
        isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDown::sInstance)) {
        if ((KoopaFunction::isKoopaLv1(mHost) || KoopaFunction::isKoopaLv2(mHost)) && MR::isGreaterStep(this, 45)) {
            return true;
        }

        if (KoopaFunction::isKoopaLv3(mHost) && MR::isGreaterStep(this, 20)) {
            return true;
        }
    }

    return isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDownEnd::sInstance);
}

bool KoopaStateDamageEscape::tryDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if ((!MR::isMsgPlayerSpinAttack(msg) || !MR::isSensorPlayer(pSender)) && !MR::isMsgToEnemyAttackBlow(msg)) {
        return false;
    }

    if (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeRun::sInstance) ||
        isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeFind::sInstance) ||
        isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeEnd::sInstance)) {
        KoopaFunction::startKoopaSpinHitBlow(mHost);
        KoopaFunction::changeBgmStateDamageTail();
        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunStart::sInstance);

        return true;
    }

    if (MR::isSensor(pReceiver, "DamageEscapeBody")) {
        if ((isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunStart::sInstance) && MR::isGreaterStep(this, 60)) ||
            isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRun::sInstance) ||
            (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunEnd::sInstance) && MR::isLessStep(this, 60)) ||
            isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunFinal::sInstance) ||
            (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunEndFinal::sInstance) && MR::isLessStep(this, 60))) {
            MR::tryRumblePadVeryStrong(this, 0);
            MR::startSpinHitSound(mHost);
            MR::startBlowHitSound(mHost);
            MR::startSound(mHost, "SE_BM_KOOPA_DAMAGE_L");

            if (mIsLastHit && (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunStart::sInstance) ||
                               isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRun::sInstance) ||
                               isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunEnd::sInstance))) {
                mDamageTailRunParam = &::sDamageTailRunFinalParam;
                mMaxRunFrames = ::sDamageTailStepFinal;

                MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_2");
                MR::startSound(mHost, "SE_BV_KOOPA_FLIP_DAMAGE_M");
                MR::stopSceneForDefaultHit(10);

                setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunStartFinal::sInstance);

                return true;
            }

            KoopaFunction::changeBgmStateFlyAway();

            if (mIsLastHit && (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunFinal::sInstance) ||
                               isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunEndFinal::sInstance))) {
                MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT");
                MR::startSystemSE("SE_SY_VS_KOOPA_LAST_HIT");
                MR::startSound(mHost, "SE_BV_KOOPA_FLIP_DAMAGE_L");
            } else {
                MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_3");
                MR::startSound(mHost, "SE_BV_KOOPA_FLIP_DAMAGE_M");
            }

            MR::stopSceneForDefaultHit(20);
            setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDown::sInstance);

            return true;
        }
    }

    return false;
}

bool KoopaStateDamageEscape::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if ((isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeRun::sInstance) ||
             (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRun::sInstance)) &&
                 !(KoopaFunction::isKoopaVs1(mHost) && KoopaFunction::isKoopaLv1(mHost)) ||
             isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunFinal::sInstance)) &&
            MR::isSensor(pSender, "DamageEscapePush") && MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFlipMaximum(pReceiver, pSender)) {
            return true;
        }

        if (KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver)) {
            return true;
        }

        if (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeRun::sInstance) &&
            KoopaFunction::tryKoopaShellAttackPlayer(pSender, pReceiver)) {
            return true;
        }
    }

    if (MR::isSensor(pSender, "DamageEscapePush") && KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
        return true;
    }

    return false;
}

void KoopaStateDamageEscape::exeEscapeStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "EscapeRunStart");
        MR::setVelocityJump(mHost, ::sEscapeStartJumpSpeed);

        KoopaFunction::startKoopaCamera(mHost, "逃走開始");
        KoopaFunction::startBreakKoopaTailThorn(mHost);

        MR::startSound(mHost, "SE_BM_KOOPA_BURN");
        MR::startSound(mHost, "SE_BV_KOOPA_BURN_START");
    }

    KoopaFunction::escapeKoopaFromPlayer(mHost, ::sEscapeStartParam);

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeStartLoop::sInstance);
    }
}

void KoopaStateDamageEscape::exeEscapeStartLoop() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "EscapeRunStartLoop");
    }

    if (!KoopaFunction::escapeKoopaFromPlayer(mHost, ::sEscapeStartParam) && MR::isBindedGround(mHost)) {
        MR::tryRumblePadAndCameraDistanceStrong(mHost, 800.0f, 1200.0f, 2000.0f);
        MR::startSound(mHost, "SE_BM_KOOPA_LAND");

        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeStartLand::sInstance);
    }
}

void KoopaStateDamageEscape::exeEscapeStartLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "EscapeRunStartLand");
    }

    KoopaFunction::escapeKoopaFromPlayer(mHost, ::sEscapeStartParam);

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeRun::sInstance);
    }
}

void KoopaStateDamageEscape::exeEscapeRun() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "EscapeRun");
        KoopaFunction::startKoopaCamera(mHost, "逃走");
    }

    KoopaFunction::escapeKoopaFromPlayer(mHost, *mEscapeRunParam);

    mEscapeTime++;

    if (mEscapeTime >= mMaxEscapeTime) {
        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeEnd::sInstance);
        return;
    }

    if (KoopaFunction::isKoopaSightPlayer(mHost, ::sEscapeFindSight)) {
        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeFind::sInstance);
        return;
    }

    if (MR::isBindedWall(mHost)) {
        MR::addVelocityJump(mHost, ::sEscapeRunWallJumpSpeed);
        return;
    }
}

void KoopaStateDamageEscape::exeEscapeFind() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "EscapeTurn");
    }

    KoopaFunction::escapeKoopaFromPlayer(mHost, ::sEscapeFindParam);

    mEscapeTime++;

    if (MR::isActionEnd(mHost)) {
        TVec3f* front = KoopaFunction::getKoopaFrontPtr(mHost);
        front->x *= -1.0f;
        front->y *= -1.0f;
        front->z *= -1.0f;

        MR::startAction(mHost, "EscapeTurnEnd");
        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvEscapeRun::sInstance);
    }
}

void KoopaStateDamageEscape::exeEscapeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "EscapeEnd");
    }

    KoopaFunction::escapeKoopaFromPlayer(mHost, ::sEscapeEndParam);

    if (MR::isActionEnd(mHost)) {
        KoopaFunction::startRecoverKoopaTailThorn(mHost);
        kill();
    }
}

void KoopaStateDamageEscape::exeDamageTailRunStart() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunStart::sInstance)) {
            MR::startAction(mHost, "DamageTailRunStart");
        } else {
            MR::startAction(mHost, "DamageTailRunFastStart");
        }

        MR::calcVecFromPlayerH(KoopaFunction::getKoopaFrontPtr(mHost), mHost);

        Koopa* pKoopa = mHost;

        MR::setVelocity(pKoopa, KoopaFunction::getKoopaFront(pKoopa) * mRotateVelocity);

        KoopaFunction::startKoopaCamera(mHost, "逃走（尻尾ダメージ）");

        MR::invalidateHitSensor(mHost, "ReceiverTail");
        MR::invalidateHitSensor(mHost, "ReceiverTailTop");
        MR::validateHitSensor(mHost, "DamageEscapeBody");
        MR::validateHitSensor(mHost, "DamageEscapePush");
    }

    if (MR::isBindedWall(mHost)) {
        MR::addVelocityJump(mHost, ::sDamageTailWallJumpSpeed);
    }

    KoopaFunction::escapeKoopaFromPlayer(mHost, *mDamageTailRunParam);
    MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_SWOON");

    if (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunStart::sInstance)) {
        MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_TAIL_RUN");
    } else {
        MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_TAIL_RUN_FAST");
    }

    if (MR::isStep(this, ::sDamageTailRunStartBreakStep)) {
        KoopaFunction::startBreakKoopaArmor(mHost);
    }

    if (MR::isActionEnd(mHost)) {
        if (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunStart::sInstance)) {
            setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRun::sInstance);
        } else {
            setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunFinal::sInstance);
        }
    }
}

void KoopaStateDamageEscape::exeDamageTailRun() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRun::sInstance)) {
            MR::startAction(mHost, "DamageTailRun");
        } else {
            MR::startAction(mHost, "DamageTailRunFast");
        }

        mIsTurnClockwise = MR::isHalfProbability();
    }

    if (MR::isBindedWall(mHost)) {
        MR::addVelocityJump(mHost, ::sDamageTailWallJumpSpeed);
    }

    KoopaFunction::escapeKoopaFromPlayer(mHost, *mDamageTailRunParam);
    MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_SWOON");

    if (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRun::sInstance)) {
        MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_TAIL_RUN");
    } else {
        MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_TAIL_RUN_FAST");
    }

    if (mIsTurnClockwise) {
        Koopa* pKoopa = mHost;
        MR::rotateVecDegree(KoopaFunction::getKoopaFrontPtr(pKoopa), -pKoopa->mGravity, ::sDamageTailTurnDegree);
    } else {
        Koopa* pKoopa = mHost;
        MR::rotateVecDegree(KoopaFunction::getKoopaFrontPtr(pKoopa), -pKoopa->mGravity, -::sDamageTailTurnDegree);
    }

    if (MR::isStep(this, mMaxRunFrames)) {
        if (isNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRun::sInstance)) {
            setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunEnd::sInstance);
        } else {
            setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDamageTailRunEndFinal::sInstance);
        }
    }
}

void KoopaStateDamageEscape::exeDamageTailRunEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "DamageTailRunEnd");
    }

    KoopaFunction::escapeKoopaFromPlayer(mHost, ::sDamageTailRunEndParam);

    if (MR::isStep(this, ::sDamageTailRunEndRecoverStep)) {
        KoopaFunction::startRecoverKoopaArmor(mHost);
        KoopaFunction::startRecoverKoopaTailThorn(mHost);
    }

    if (MR::isActionEnd(mHost)) {
        MR::zeroVelocity(mHost);
        kill();
    }
}

void KoopaStateDamageEscape::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Down");
        MR::setVelocityJumpAwayFromPlayer(mHost, 35.0f, mJumpAwayVelocity);
        KoopaFunction::startKoopaCamera(mHost, "逃走ダウン");
        MR::emitEffect(mHost, "DownSmoke");
    }

    MR::addVelocityToGravity(mHost, ::sDownGravity);

    Koopa* pKoopa = mHost;
    TVec3f velocity = pKoopa->mVelocity;
    MR::vecKillElement(velocity, pKoopa->mGravity, &velocity);

    if (!MR::isNearZero(velocity)) {
        MR::normalize(&velocity);
        KoopaFunction::getKoopaFrontPtr(mHost)->set(-velocity);
    }

    pKoopa = mHost;
    if (!MR::sendMsgEnemyAttackToBindedSensor(pKoopa, pKoopa->getSensor("Body")) && !MR::isFirstStep(this) && MR::isBindedGround(mHost)) {
        MR::tryRumblePadAndCameraDistanceStrong(mHost, 1500.0f, 2000.0f, 2000.0f);
        MR::startSound(mHost, "SE_BM_KOOPA_LAND");

        if (!MR::sendMsgToBindedSensor(ACTMES_KOOPA_PLATE_DAMAGE, KoopaFunction::getKoopaMessageSensor(mHost))) {
            pKoopa = mHost;
            if (MR::sendMsgToBindedSensor(ACTMES_KOOPA_HIP_DROP_ATTACK, pKoopa, pKoopa->getSensor("Body"))) {
                MR::stopScene(::sDownStopSceneStep);
                return;
            }
        }

        MR::emitEffect(mHost, "DownSmoke");
        MR::zeroVelocity(mHost);

        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDownLand::sInstance);
    }
}

void KoopaStateDamageEscape::exeDownLand() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::changeBgmStateNormal(120);
        MR::startAction(mHost, "DownLand");

        if (KoopaFunction::isKoopaVs3(mHost) && !KoopaFunction::isKoopaLv3(mHost) && !KoopaFunction::isKoopaAngry(mHost)) {
            MR::appearStarPieceToDirection(mHost, mHost->mPosition, -mHost->mGravity, 10, 50.0f, 60.0f, false);
            MR::startSound(mHost, "SE_OJ_STAR_PIECE_BURST");
        }
    }

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvKoopaStateDamageEscape::KoopaStateDamageEscapeNrvDownEnd::sInstance);
    }
}

void KoopaStateDamageEscape::exeDownEnd() {
}

KoopaStateDamageEscape::~KoopaStateDamageEscape() {
}
