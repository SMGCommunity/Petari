#include "Game/Boss/KoopaStateChaseRoll.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFigureBall.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaRockBreak.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static MR::ActorMoveParam sStartParam = {0.0f, 0.5f, 1.0f, 3.0f};
    static MR::ActorMoveParam sChaseRollParam = {1.1f, 2.0f, 0.95f, 1.5f};
    static MR::ActorMoveParam sChaseRollParamFast = {1.5f, 2.0f, 0.95f, 1.5f};
    static MR::ActorMoveParam sEndParam = {0.0f, 1.0f, 0.98f, 1.0f};
    static const s32 sWaitToStartStep = 60;
    static const s32 sWaitToStartStepAngry = 60;
    static const s32 sStartStepToJump = 24;
    static const f32 sStartJumpSpeed = 30.0f;
    static const f32 sRotateSpeed = 5.0f;
    static const s32 sStepRollGround = 900;
    static const f32 sEndJumpSpeed = 30.0f;
};  // namespace

namespace NrvKoopaStateChaseRoll {
    NEW_NERVE(KoopaStateChaseRollNrvWaitToStart, KoopaStateChaseRoll, WaitToStart);
    NEW_NERVE(KoopaStateChaseRollNrvStart, KoopaStateChaseRoll, Start);
    NEW_NERVE(KoopaStateChaseRollNrvRollAir, KoopaStateChaseRoll, RollAir);
    NEW_NERVE(KoopaStateChaseRollNrvRollGround, KoopaStateChaseRoll, RollGround);
    NEW_NERVE(KoopaStateChaseRollNrvEndAir, KoopaStateChaseRoll, EndAir);
    NEW_NERVE(KoopaStateChaseRollNrvEndLand, KoopaStateChaseRoll, EndLand);
};  // namespace NrvKoopaStateChaseRoll

KoopaStateChaseRoll::KoopaStateChaseRoll(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[転がり追跡攻撃]", pKoopa), mFigureBall(), mRollDelay(::sWaitToStartStep) {
}

void KoopaStateChaseRoll::init() {
    initNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvStart::sInstance);

    KoopaFunction::initKoopaCamera(mHost, "ロール追跡開始");

    KoopaFunction::createKoopaRock(mHost);

    mFigureBall = new KoopaFigureBall("追跡ボール", mHost, 260.0f, &::sChaseRollParam);
    mFigureBall->initWithoutIter();

    kill();
}

void KoopaStateChaseRoll::appear() {
    MR::zeroVelocity(mHost);

    mIsDead = false;

    MR::validateHitSensor(mHost, "ChaseRollFace");
    MR::validateHitSensor(mHost, "ChaseRollAttack");
    MR::validateHitSensor(mHost, "ChaseRollStarPiece");

    if (KoopaFunction::isKoopaAngry(mHost)) {
        mFigureBall->mMoveParam = &::sChaseRollParamFast;
        mRollDelay = ::sWaitToStartStepAngry;
    } else {
        mFigureBall->mMoveParam = &::sChaseRollParam;
        mRollDelay = ::sWaitToStartStep;
    }

    if (mRollDelay >= 0) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvWaitToStart::sInstance);
    } else {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvStart::sInstance);
    }
}

void KoopaStateChaseRoll::kill() {
    mIsDead = true;

    MR::invalidateHitSensor(mHost, "ChaseRollFace");
    MR::invalidateHitSensor(mHost, "ChaseRollAttack");
    MR::invalidateHitSensor(mHost, "ChaseRollStarPiece");

    MR::tryDeleteEffect(KoopaFunction::getKoopaRock(mHost), "RollingSmoke");
    KoopaFunction::getKoopaRock(mHost)->kill();

    KoopaFunction::endKoopaCamera(mHost, "ロール追跡開始", false, -1);

    KoopaFunction::startFaceCtrl(mHost);
}

bool KoopaStateChaseRoll::tryCalcAndSetBaseMtx() {
    if (isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollAir::sInstance) ||
        isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollGround::sInstance)) {
        Koopa* pKoopa = mHost;
        MR::setBaseTRMtx(pKoopa, mFigureBall->getBaseMtx());

        return true;
    }

    return false;
}

bool KoopaStateChaseRoll::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && MR::isSensor(pSender, "ChaseRollAttack") && MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
        return true;
    }

    if (MR::isSensor(pSender, "ChaseRollAttack") && KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
        return true;
    }

    return false;
}

bool KoopaStateChaseRoll::tryDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgPlayerSpinAttack(msg) || !MR::isSensor(pReceiver, "ChaseRollFace")) {
        return false;
    }

    if (isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollGround::sInstance)) {
        KoopaRockBreak* pKoopaRockBreak = KoopaFunction::getKoopaRockBreak(mHost);
        pKoopaRockBreak->mCalcOwnMtx = true;
        KoopaFunction::getKoopaRockBreak(mHost)->appear();
        MR::calcAnimDirect(KoopaFunction::getKoopaRockBreak(mHost));

        MR::startAction(KoopaFunction::getKoopaRockBreak(mHost), "Break");
        MR::startSound(mHost, "SE_BM_KOOPA_ROCK_BREAK");

        pKoopaRockBreak = KoopaFunction::getKoopaRockBreak(mHost);
        pKoopaRockBreak->mCalcOwnMtx = false;

        return true;
    }

    return false;
}

bool KoopaStateChaseRoll::isEnableGuard() const {
    if (isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvWaitToStart::sInstance) ||
        isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndAir::sInstance) ||
        isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndLand::sInstance)) {
        return true;
    }

    return false;
}

void KoopaStateChaseRoll::exeWaitToStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Wait");
    }

    if (MR::isStep(this, mRollDelay)) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvStart::sInstance);
    }
}

void KoopaStateChaseRoll::exeStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::getKoopaRock(mHost)->appear();
        KoopaFunction::getKoopaRockBreak(mHost)->appear();

        KoopaRockBreak* pKoopaRockBreak = KoopaFunction::getKoopaRockBreak(mHost);
        pKoopaRockBreak->mCalcOwnMtx = true;

        MR::startAction(mHost, "ChaseRollStart");
        MR::startAction(KoopaFunction::getKoopaRock(mHost), "AttackRollStart");
        MR::startAction(KoopaFunction::getKoopaRockBreak(mHost), "AttackRollStart");

        KoopaFunction::startKoopaCamera(mHost, "ロール追跡開始");
        KoopaFunction::endFaceCtrl(mHost, -1);
    }

    MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_ROCK_GATHER");

    if (MR::isStep(this, ::sStartStepToJump)) {
        MR::setVelocityJump(mHost, ::sStartJumpSpeed);
    }

    if (MR::isGreaterStep(this, ::sStartStepToJump)) {
        MR::moveAndTurnToPlayer(mHost, &mHost->mFront, ::sStartParam);
    }

    if (MR::isActionEnd(mHost)) {
        mFigureBall->appear();
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollAir::sInstance);
    } else if (MR::isPlayerDamaging()) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndAir::sInstance);
    }
}

void KoopaStateChaseRoll::exeRollAir() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "ChaseRoll");
        MR::startAction(KoopaFunction::getKoopaRock(mHost), "AttackRoll");
    }

    mFigureBall->mAngle += ::sRotateSpeed;
    mFigureBall->movement();

    mHost->mPosition.set(mFigureBall->mPosition);

    if (MR::isBindedGround(mFigureBall)) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollGround::sInstance);
    } else if (MR::isPlayerDamaging()) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndAir::sInstance);
    }
}

void KoopaStateChaseRoll::exeRollGround() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::endKoopaCamera(mHost, "ロール追跡開始", false, -1);
        MR::emitEffect(KoopaFunction::getKoopaRock(mHost), "RollingSmoke");
    }

    mFigureBall->movement();

    mHost->mPosition.set(mFigureBall->mPosition);

    MR::sendMsgEnemyAttackToBindedSensor(mFigureBall, mHost->getSensor("ChaseRollFace"));

    MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_CHACE_ROLL");

    if (MR::isPlayerDamaging() || MR::isGreaterStep(this, ::sStepRollGround)) {
        MR::deleteEffect(KoopaFunction::getKoopaRock(mHost), "RollingSmoke");

        mFigureBall->kill();

        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndAir::sInstance);
    }
}

void KoopaStateChaseRoll::exeEndAir() {
    if (MR::isFirstStep(this)) {
        MR::setVelocityJump(mHost, ::sEndJumpSpeed);

        MR::startAction(mHost, "ChaseRollEnd");
        MR::startAction(KoopaFunction::getKoopaRock(mHost), "AttackRollEnd");

        KoopaRockBreak* pKoopaRockBreak = KoopaFunction::getKoopaRockBreak(mHost);
        pKoopaRockBreak->mCalcOwnMtx = true;

        KoopaFunction::getKoopaRockBreak(mHost)->appear();
        MR::calcAnimDirect(KoopaFunction::getKoopaRockBreak(mHost));
        MR::startAction(KoopaFunction::getKoopaRockBreak(mHost), "AttackRollEnd");

        pKoopaRockBreak = KoopaFunction::getKoopaRockBreak(mHost);
        pKoopaRockBreak->mCalcOwnMtx = false;

        MR::startSound(mHost, "SE_BM_KOOPA_ROCK_BREAK");
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, ::sEndParam);

    if (!MR::isFirstStep(this) && MR::isBindedGround(mHost)) {
        MR::zeroVelocity(mHost);
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndLand::sInstance);
    }
}

void KoopaStateChaseRoll::exeEndLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "ChaseRollLand");
    }

    if (MR::isBckStopped(mHost)) {
        kill();
    }
}

KoopaStateChaseRoll::~KoopaStateChaseRoll() {
}
