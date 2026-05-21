#include "Game/Boss/KoopaStateAttackShockWave.hpp"
#include "Game/Boss/KoopaFunction.hpp"

namespace NrvKoopaStateAttackShockWave {
    NEW_NERVE(KoopaStateAttackShockWaveNrvJumpStart, KoopaStateAttackShockWave, JumpStart);
    NEW_NERVE(KoopaStateAttackShockWaveNrvJump, KoopaStateAttackShockWave, Jump);
    NEW_NERVE(KoopaStateAttackShockWaveNrvJumpFast, KoopaStateAttackShockWave, JumpFast);
    NEW_NERVE(KoopaStateAttackShockWaveNrvLand, KoopaStateAttackShockWave, Land);
}  // namespace NrvKoopaStateAttackShockWave

KoopaStateAttackShockWave::KoopaStateAttackShockWave(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[衝撃波攻撃]", pKoopa), _10(1), _14(), mJumpVelocity(40.0f), mGravity(1.5f), mJumpDelay(15) {
}

KoopaStateAttackShockWave::~KoopaStateAttackShockWave() {
}

void KoopaStateAttackShockWave::init() {
    initNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpStart::sInstance);
    kill();
}

void KoopaStateAttackShockWave::appear() {
    mIsDead = false;
    _14 = 0;

    MR::validateHitSensor(mHost, "AttackHipDrop");

    if (KoopaFunction::isKoopaVs1(mHost)) {
        if (KoopaFunction::isKoopaLv3(mHost)) {
            _10 = 3;
        } else if (KoopaFunction::isKoopaLv2(mHost)) {
            _10 = 2;
        } else {
            _10 = 1;
        }

        mJumpVelocity = 40.0f;
        mGravity = 1.5f;
        mJumpDelay = 15;
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        if (KoopaFunction::isKoopaLv3(mHost)) {
            _10 = 5;
        } else {
            _10 = 3;
        }

        mJumpVelocity = 40.0f;
        mGravity = 1.5;
        mJumpDelay = 10;
    } else if (KoopaFunction::isKoopaLv1(mHost)) {
        _10 = 3;

        if (KoopaFunction::isKoopaAngry(mHost)) {
            mJumpVelocity = 40.0f;
            mGravity = 1.5f;
            mJumpDelay = 5;
        } else {
            mJumpVelocity = 40.0f;
            mGravity = 2.0f;
            mJumpDelay = 10;
        }
    } else {
        mJumpVelocity = 40.0f;
        mGravity = 2.0f;

        if (KoopaFunction::isKoopaAngry(mHost)) {
            _10 = 5;
            mJumpDelay = 5;
        } else {
            _10 = 3;
            mJumpDelay = 10;
        }
    }

    setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpStart::sInstance);
}

void KoopaStateAttackShockWave::kill() {
    MR::invalidateHitSensor(mHost, "AttackHipDrop");

    mIsDead = true;
}

bool KoopaStateAttackShockWave::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
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
        if (_14 == _10 - 1) {
            MR::startAction(mHost, "ShockWaveJump3rd");
        } else {
            isNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpFast::sInstance) ? MR::startAction(mHost, "JumpFast") :
                                                                                                      MR::startAction(mHost, "ShockWaveJump");
        }

        MR::startSound(mHost, "SE_BM_KOOPA_JUMP", -1, -1);

        _14 == 0 ? MR::setVelocityJump(mHost, 50.0f) : MR::setVelocityJump(mHost, mJumpVelocity);
    }

    MR::addVelocityToGravity(mHost, mGravity);

    if (KoopaFunction::isKoopaVs3(mHost) && KoopaFunction::isKoopaLv3(mHost)) {
        Koopa* pKoopa = mHost;
        MR::turnDirectionToPlayerDegree(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), 3.0f);
    }

    Koopa* pKoopa = mHost;
    if (!MR::sendMsgEnemyAttackToBindedSensor(pKoopa, pKoopa->getSensor("Body")) && MR::isOnGround(mHost)) {
        MR::tryRumblePadAndCameraDistanceMiddle(mHost, 1500.0f, 2500.0f, 2000.0f);
        MR::shakeCameraWeak();

        MR::startSound(mHost, "SE_BM_KOOPA_LAND", -1, -1);

        KoopaFunction::emitKoopaShockWave(mHost);

        mHost->mVelocity.set2(0.0f);

        setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvLand::sInstance);
    }
}

void KoopaStateAttackShockWave::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "ShockWaveLand");
    }

    if (MR::isStep(this, mJumpDelay) && (_14++, _14 < _10)) {
        setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpFast::sInstance);
    } else {
        if (MR::isActionEnd(mHost)) {
            kill();
        }
    }
}

void KoopaStateAttackShockWave::exeJumpFast() {
}
