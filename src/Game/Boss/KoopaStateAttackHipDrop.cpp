#include "Game/Boss/KoopaStateAttackHipDrop.hpp"
#include "Game/Boss/KoopaFunction.hpp"

namespace {
    MR::ActorMoveParam sParamRunStart = {3.0f, 1.0f, 0.9f, 3.0f};
    MR::ActorMoveParam sParamJumpStart = {0.0f, 1.0f, 0.95f, 0.0f};
    MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};
}  // namespace

namespace MR {
    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* pVec, const MR::ActorMoveParam& rMoveParam) {
        MR::moveAndTurnToPlayer(pActor, pVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    }
}  // namespace MR

namespace NrvKoopaStateAttackHipDrop {
    NEW_NERVE(KoopaStateAttackHipDropNrvDamage, KoopaStateAttackHipDrop, Damage);
    NEW_NERVE(KoopaStateAttackHipDropNrvLand, KoopaStateAttackHipDrop, Land);
    NEW_NERVE(KoopaStateAttackHipDropNrvJumpAttackDown, KoopaStateAttackHipDrop, JumpAttackDown);
    NEW_NERVE(KoopaStateAttackHipDropNrvJumpWaitAir, KoopaStateAttackHipDrop, JumpWaitAir);
    NEW_NERVE(KoopaStateAttackHipDropNrvJumpUp, KoopaStateAttackHipDrop, JumpUp);
    NEW_NERVE(KoopaStateAttackHipDropNrvJumpStart, KoopaStateAttackHipDrop, JumpStart);
    NEW_NERVE(KoopaStateAttackHipDropNrvRun, KoopaStateAttackHipDrop, Run);
}  // namespace NrvKoopaStateAttackHipDrop

KoopaStateAttackHipDrop::KoopaStateAttackHipDrop(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[ヒップドロップ攻撃]", pKoopa), _10(0.0f, 0.0f, 0.0f), _1C(1), _20(), _24(30), _28() {
}

KoopaStateAttackHipDrop::~KoopaStateAttackHipDrop() {
}

void KoopaStateAttackHipDrop::init() {
    KoopaFunction::initKoopaCamera(mHost, "ヒップドロップ攻撃");
    KoopaFunction::initKoopaCamera(mHost, "ヒップドロップ着地");
    KoopaFunction::initKoopaCamera(mHost, "ヒップドロップ着地（マリオ痺れ）");

    initNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvRun::sInstance);

    kill();
}

void KoopaStateAttackHipDrop::appear() {
    MR::onCalcGravity(mHost);
    MR::onBind(mHost);

    mIsDead = false;

    MR::validateHitSensor(mHost, "AttackHipDrop");

    _20 = 0;

    if (KoopaFunction::isKoopaVs1(mHost)) {
        _1C = 3;
        _24 = 30;

        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvRun::sInstance);
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        _1C = 2;
        _24 = 15;

        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpUp::sInstance);
    } else {
        _24 = 15;

        KoopaFunction::isKoopaAngry(mHost) ? _1C = 2 : _1C = 3;

        MR::isNearPlayer(mHost, 2000.0f) ? setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpUp::sInstance) :
                                           setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvDamage::sInstance);
    }
}

void KoopaStateAttackHipDrop::kill() {
    KoopaFunction::endKoopaCamera(mHost, "ヒップドロップ攻撃", false, -1);
    KoopaFunction::endKoopaCamera(mHost, "ヒップドロップ着地", false, -1);
    KoopaFunction::endKoopaCamera(mHost, "ヒップドロップ着地（マリオ痺れ）", false, -1);

    MR::invalidateHitSensor(mHost, "AttackHipDrop");

    mIsDead = true;
}

bool KoopaStateAttackHipDrop::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpAttackDown::sInstance) && MR::isSensor(pSender, "AttackHipDrop")) {
        if (MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
            return true;
        }

        if (MR::sendArbitraryMsg(192, pReceiver, pSender)) {
            return true;
        }
    }

    if (KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver)) {
        return false;
    }

    return KoopaFunction::tryKoopaBodyAttackPlayerMaximum(pSender, pReceiver);
}

bool KoopaStateAttackHipDrop::isDamage() const {
    return isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvDamage::sInstance);
}

bool KoopaStateAttackHipDrop::isEnableGuard() const {
    if (isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvDamage::sInstance) ||
        isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpStart::sInstance) ||
        isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpUp::sInstance) ||
        isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvLand::sInstance)) {
        return true;
    }

    return false;
}

void KoopaStateAttackHipDrop::exeRun() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Run");
        KoopaFunction::startKoopaCamera(mHost, "ヒップドロップ攻撃");
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sParamRunStart);

    Koopa* pKoopa = mHost;
    if (MR::sendMsgEnemyAttackToBindedSensor(pKoopa, pKoopa->getSensor("Body"))) {
        return;
    }

    if (MR::isBindedWall(mHost)) {
        MR::addVelocityJump(mHost, 10.0f);
    }

    if ((MR::isNearPlayer(mHost, 1000.0f) || MR::isGreaterStep(this, 180)) && MR::calcGravitySpeed(mHost) >= 0.0f) {
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpStart::sInstance);
    }
}

void KoopaStateAttackHipDrop::exeJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "HipDropJumpStart");
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sParamJumpStart);

    if (MR::isBckStopped(mHost)) {
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpUp::sInstance);
    }
}

void KoopaStateAttackHipDrop::exeJumpUp() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "HipDropJump");
        MR::startSound(mHost, "SE_BM_KOOPA_JUMP", -1, -1);

        MR::vecKillElement(mHost->mVelocity, mHost->mGravity, &mHost->mVelocity);
        MR::restrictVelocity(mHost, 20.0f);
        MR::addVelocityJump(mHost, 50.0f);

        KoopaFunction::startKoopaCamera(mHost, "ヒップドロップ攻撃");
    }

    MR::addVelocityToGravity(mHost, 1.0f);

    Koopa* pKoopa = mHost;
    MR::turnDirectionToPlayerDegree(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), 3.0f);

    if (mHost->mVelocity.dot(mHost->mGravity) >= 0.0f) {
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpWaitAir::sInstance);
    }
}

void KoopaStateAttackHipDrop::exeJumpWaitAir() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(mHost);
    }

    MR::turnDirectionToPlayerDegree(mHost, &mHost->mFront, 2.0f);

    if (MR::isStep(this, 0)) {
        MR::getPlayerGroundPos(&_10);
    }

    if (MR::isStep(this, _24)) {
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpAttackDown::sInstance);
    }
}

void KoopaStateAttackHipDrop::exeJumpAttackDown() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "HipDropAttackDown");

        TVec3f difference = *_10;
        difference.subInline(mHost->mPosition);

        _28 = (PSVECMag(difference) / 35.0f) + 10;

        MR::normalizeOrZero(&difference);
        difference.scaleInline(35.0f);

        MR::setVelocity(mHost, difference);

        MR::startSound(mHost, "SE_BV_KOOPA_HIPDROP", -1, -1);
    }

    if (MR::isBindedGround(mHost) || MR::isGreaterStep(this, _28)) {
        Koopa* pKoopa = mHost;
        if (!MR::sendMsgEnemyAttackToBindedSensor(pKoopa, pKoopa->getSensor("Body"))) {
            if (MR::isBindedGround(mHost)) {
                pKoopa = mHost;
                if (MR::sendMsgToBindedSensor(192, pKoopa, pKoopa->getSensor("Body"))) {
                    MR::startSound(mHost, "SE_BM_KOOPA_LAND", -1, -1);
                    MR::stopScene(15);

                    if (MR::sendMsgToBindedSensor(194, KoopaFunction::getKoopaMessageSensor(mHost))) {
                        MR::zeroVelocity(mHost);

                        MR::tryRumblePadAndCameraDistanceVeryStrong(mHost, 1500.0f, 3000.0f, 2000.0f);

                        MR::startSound(mHost, "SE_OJ_KOOPA_PLATE_LAVA_ST", -1, -1);

                        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvDamage::sInstance);
                    } else {
                        MR::tryRumblePadAndCameraDistanceMiddle(mHost, 1500.0f, 3000.0f, 2000.0f);
                    }

                    return;
                }
            }

            MR::zeroVelocity(mHost);
            MR::tryRumblePadAndCameraDistanceStrong(mHost, 1500.0f, 3000.0f, 2000.0f);

            KoopaFunction::emitKoopaShockWave(mHost);

            KoopaFunction::startKoopaCamera(mHost, "ヒップドロップ着地");
            MR::startSound(mHost, "SE_BM_KOOPA_LAND", -1, -1);

            setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvLand::sInstance);
        }
    }
}

void KoopaStateAttackHipDrop::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "HipDropLand");
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sFallParam);

    if (MR::isBckStopped(mHost)) {
        _20++;

        if (_20 < _1C && MR::isPlayerDamaging()) {
            kill();
        } else {
            setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvRun::sInstance);
        }
    }
}

void KoopaStateAttackHipDrop::exeDamage() {
}
