#include "Game/Boss/KoopaStateAttackRoll.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFigureBall.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static MR::ActorMoveParam sStartParam = {0.0f, 0.5f, 1.0f, 3.0f};
    static MR::ActorMoveParam sAttackRollParamLv2Slow = {3.0f, 2.0f, 0.9f, 1.0f};
    static MR::ActorMoveParam sAttackRollParamLv2Fast = {4.5f, 2.0f, 0.9f, 1.0f};
    static MR::ActorMoveParam sAttackRollParamLv3Slow = {8.0f, 2.0f, 0.8f, 0.5f};
    static MR::ActorMoveParam sAttackRollParamLv3Fast = {10.0f, 2.0f, 0.8f, 1.0f};
    static MR::ActorMoveParam sEndParam = {0.0f, 1.0f, 0.95f, 1.0f};
    static const s32 sWaitToStartStep = 90;
    static const s32 sWaitToStartStepVs3Lv2 = 30;
    static const s32 sWaitToStartStepVs3Angry = 75;
    static const s32 sStartStepToJump = 24;
    static const f32 sStartJumpSpeed = 20.0f;
    static const f32 sRotateSpeed = 5.0f;
    static const s32 sRollGroundStep = 600;
    static const s32 sRollGroundStepVs3Lv2Angry = 300;
    static const s32 sRollGroundStepVs3Lv3 = 280;
    static const s32 sRollGroundStepVs3Lv3Angry = 320;
    static const f32 sEndJumpSpeed = 30.0f;
};  // namespace

namespace NrvKoopaStateAttackRoll {
    NEW_NERVE(KoopaStateAttackRollNrvWaitToStart, KoopaStateAttackRoll, WaitToStart);
    NEW_NERVE(KoopaStateAttackRollNrvStart, KoopaStateAttackRoll, Start);
    NEW_NERVE(KoopaStateAttackRollNrvRollAir, KoopaStateAttackRoll, RollAir);
    NEW_NERVE(KoopaStateAttackRollNrvRollGround, KoopaStateAttackRoll, RollGround);
    NEW_NERVE(KoopaStateAttackRollNrvEndAir, KoopaStateAttackRoll, EndAir);
    NEW_NERVE(KoopaStateAttackRollNrvEndLand, KoopaStateAttackRoll, EndLand);
};  // namespace NrvKoopaStateAttackRoll

KoopaStateAttackRoll::KoopaStateAttackRoll(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[ローリング攻撃]", pKoopa), mFigureBall(), mRollDelay(::sWaitToStartStep), mRollTime(::sRollGroundStep), _1C(),
      _1D() {
}

void KoopaStateAttackRoll::init() {
    initNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvStart::sInstance);

    KoopaFunction::initKoopaCamera(mHost, "ローリング攻撃開始");
    KoopaFunction::initKoopaCamera(mHost, "ローリング攻撃");

    KoopaFunction::createKoopaRollBall(mHost);

    mFigureBall = new KoopaFigureBall("追跡ボール", mHost, 230.0f, &::sAttackRollParamLv3Slow);
    mFigureBall->initWithoutIter();

    kill();
}

void KoopaStateAttackRoll::appear() {
    MR::zeroVelocity(mHost);

    mIsDead = false;

    MR::validateHitSensor(mHost, "AttackRoll");
    MR::validateHitSensor(mHost, "AttackRollReceiver");
    MR::validateHitSensor(mHost, "AttackRollStarPiece");

    _1C = false;

    if (KoopaFunction::isKoopaVs3(mHost)) {
        if (KoopaFunction::isKoopaLv2(mHost)) {
            mRollDelay = ::sWaitToStartStepVs3Lv2;

            if (KoopaFunction::isKoopaAngry(mHost)) {
                mFigureBall->mMoveParam = &::sAttackRollParamLv2Fast;
                mRollTime = ::sRollGroundStepVs3Lv2Angry;
            } else {
                mFigureBall->mMoveParam = &::sAttackRollParamLv2Slow;
                mRollTime = ::sRollGroundStep;
            }
        } else {
            if (KoopaFunction::isKoopaAngry(mHost)) {
                mFigureBall->mMoveParam = &::sAttackRollParamLv3Fast;
                mRollTime = ::sRollGroundStepVs3Lv3Angry;
                mRollDelay = ::sWaitToStartStepVs3Angry;
            } else {
                mFigureBall->mMoveParam = &::sAttackRollParamLv3Slow;
                mRollTime = ::sRollGroundStepVs3Lv3;
                mRollDelay = ::sWaitToStartStep;
            }

            if (_1D) {
                _1D = false;
                mRollDelay = 0;
            }
        }
    } else {
        mFigureBall->mMoveParam = &::sAttackRollParamLv3Slow;
        mRollTime = ::sRollGroundStepVs3Lv3;
        mRollDelay = ::sWaitToStartStep;
    }

    if (mRollDelay >= 0) {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvWaitToStart::sInstance);
    } else {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvStart::sInstance);
    }
}

void KoopaStateAttackRoll::kill() {
    mIsDead = true;

    KoopaFunction::getKoopaRollBall(mHost)->kill();

    MR::invalidateHitSensor(mHost, "AttackRoll");
    MR::invalidateHitSensor(mHost, "AttackRollReceiver");
    MR::invalidateHitSensor(mHost, "AttackRollStarPiece");

    KoopaFunction::endKoopaCamera(mHost, "ローリング攻撃開始", false, -1);
    KoopaFunction::endKoopaCamera(mHost, "ローリング攻撃", false, -1);
}

bool KoopaStateAttackRoll::tryCalcAndSetBaseMtx() {
    if (isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvRollAir::sInstance) ||
        isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvRollGround::sInstance)) {
        Koopa* pKoopa = mHost;
        MR::setBaseTRMtx(pKoopa, mFigureBall->getBaseMtx());

        return true;
    }

    return false;
}

bool KoopaStateAttackRoll::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && MR::isSensor(pSender, "AttackRoll") && MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
        _1C = true;
        return true;
    }

    if (MR::isSensor(pSender, "AttackRoll") && KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
        return true;
    }

    return false;
}

bool KoopaStateAttackRoll::isDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) const {
    if (!MR::isMsgToEnemyAttackBlow(msg) || !MR::isSensor(pReceiver, "AttackRollReceiver")) {
        return false;
    }

    return !mIsDead;
}

bool KoopaStateAttackRoll::isEnableGuard() const {
    if (isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvWaitToStart::sInstance) ||
        isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndAir::sInstance) ||
        isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndLand::sInstance)) {
        return true;
    }

    return false;
}

void KoopaStateAttackRoll::exeWaitToStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Wait");
    }

    if (MR::isStep(this, mRollDelay)) {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvStart::sInstance);
    }
}

void KoopaStateAttackRoll::exeStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::getKoopaRollBall(mHost)->appear();

        MR::startAction(mHost, "AttackRollStart");
        MR::startAction(KoopaFunction::getKoopaRollBall(mHost), "AttackRollStart");

        KoopaFunction::startKoopaCamera(mHost, "ローリング攻撃開始");
    }

    if (MR::isStep(this, ::sStartStepToJump)) {
        MR::setVelocityJump(mHost, ::sStartJumpSpeed);
    }

    if (MR::isGreaterStep(this, ::sStartStepToJump)) {
        MR::moveAndTurnToPlayer(mHost, &mHost->mFront, ::sStartParam);
    }

    if (MR::isActionEnd(mHost)) {
        mFigureBall->appear();

        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvRollAir::sInstance);
    }
}

void KoopaStateAttackRoll::exeRollAir() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "AttackRoll");
        MR::startAction(KoopaFunction::getKoopaRollBall(mHost), "AttackRoll");
    }

    mFigureBall->mAngle += ::sRotateSpeed;
    mFigureBall->movement();
    mHost->mPosition.set(mFigureBall->mPosition);

    MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_ATTACK_ROLL");

    if (MR::isBindedGround(mFigureBall)) {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvRollGround::sInstance);
    } else if (MR::isPlayerDamaging()) {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndAir::sInstance);
    }
}

void KoopaStateAttackRoll::exeRollGround() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::startKoopaCamera(mHost, "ローリング攻撃");
    }

    mFigureBall->movement();
    mHost->mPosition.set(mFigureBall->mPosition);

    MR::sendMsgEnemyAttackToBindedSensor(mFigureBall, mHost->getSensor("AttackRoll"));

    MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_ATTACK_ROLL");
    MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_CHACE_ROLL");

    if (MR::isGreaterStep(this, mRollTime) || MR::isPlayerDamaging()) {
        mFigureBall->kill();

        if (!MR::isPlayerDamaging() && KoopaFunction::isKoopaVs3(mHost) && KoopaFunction::isKoopaLv3(mHost)) {
            kill();
        } else {
            setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndAir::sInstance);
        }
    }
}

void KoopaStateAttackRoll::exeEndAir() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "AttackRollEnd");
        MR::startAction(KoopaFunction::getKoopaRollBall(mHost), "AttackRollEnd");

        KoopaFunction::endKoopaCamera(mHost, "ローリング攻撃", false, -1);

        MR::setVelocityJump(mHost, ::sEndJumpSpeed);
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, ::sEndParam);

    if (!MR::isFirstStep(this) && MR::isBindedGround(mHost)) {
        MR::zeroVelocity(mHost);

        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndLand::sInstance);
    }
}

void KoopaStateAttackRoll::exeEndLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "AttackRollLand");
    }

    if (MR::isActionEnd(mHost)) {
        kill();
    }
}

KoopaStateAttackRoll::~KoopaStateAttackRoll() {
}
