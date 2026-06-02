#include "Game/Boss/KoopaBattleMain.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaStateAttackFireLong.hpp"
#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/Boss/KoopaStateAttackHipDrop.hpp"
#include "Game/Boss/KoopaStateAttackRoll.hpp"
#include "Game/Boss/KoopaStateAttackShockWave.hpp"
#include "Game/Boss/KoopaStateAttackSpin.hpp"
#include "Game/Boss/KoopaStateDamageEscape.hpp"
#include "Game/Boss/KoopaStateGuard.hpp"
#include "Game/Boss/KoopaStateJumpAway.hpp"

namespace {
    MR::ActorMoveParam sChasePlayerParam = {1.5f, 1.0f, 0.9f, 3.0f};
    MR::ActorMoveParam sWanderParam = {0.5f, 1.0f, 0.9f, 1.3f};
    MR::ActorSightParam sChasePlayerStartSight = {1800.0f, 120.0f, 50.0f};
    MR::ActorSightParam sChasePlayerKeepSight = {2200.0f, 125.0f, 50.0f};
    MR::ActorSightParam sAttackHipDropSight = {1000.0f, 90.0f, 30.0f};
}  // namespace

namespace NrvKoopaBattleMain {
    NEW_NERVE(KoopaBattleMainNrvChasePlayer, KoopaBattleMain, ChasePlayer);
    NEW_NERVE(KoopaBattleMainNrvWander, KoopaBattleMain, Wander);
    NEW_NERVE(KoopaBattleMainNrvSearch, KoopaBattleMain, Search);
    NEW_NERVE(KoopaBattleMainNrvFind, KoopaBattleMain, Find);
    NEW_NERVE(KoopaBattleMainNrvJumpAway, KoopaBattleMain, JumpAway);
    NEW_NERVE(KoopaBattleMainNrvGuard, KoopaBattleMain, Guard);
    NEW_NERVE(KoopaBattleMainNrvAttackFireShort, KoopaBattleMain, AttackFireShort);
    NEW_NERVE(KoopaBattleMainNrvAttackFireLong, KoopaBattleMain, AttackFireLong);
    NEW_NERVE(KoopaBattleMainNrvAttackShockWave, KoopaBattleMain, AttackShockWave);
    NEW_NERVE(KoopaBattleMainNrvAttackHipDrop, KoopaBattleMain, AttackHipDrop);
    NEW_NERVE(KoopaBattleMainNrvAttackSpin, KoopaBattleMain, AttackSpin);
    NEW_NERVE(KoopaBattleMainNrvAttackRoll, KoopaBattleMain, AttackRoll);
    NEW_NERVE(KoopaBattleMainNrvDamageEscape, KoopaBattleMain, DamageEscape);
}  // namespace NrvKoopaBattleMain

KoopaBattleMain::KoopaBattleMain(const char* pName, Koopa* pKoopa) : KoopaBattleBase(pName, pKoopa), _1C(), _20(), _24(), _28(), _2C(true) {
}

KoopaBattleMain::~KoopaBattleMain() {
}

void KoopaBattleMain::init() {
    KoopaBattleBase::init();
    initNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvWander::sInstance);
    MR::initActorStateKeeper(this, 16);

    _1C = new KoopaStateAttackHipDrop(mHost);
    _20 = new KoopaStateAttackShockWave(mHost);

    MR::initActorState(this, _14, &NrvKoopaBattleMain::KoopaBattleMainNrvGuard::sInstance, "Guard");
    MR::initActorState(this, _1C, &NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance, "AttackHipDrop");
    MR::initActorState(this, _20, &NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance, "AttackShockWave");
    MR::initActorState(this, new KoopaStateAttackFireShort(mHost), &NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireShort::sInstance,
                       "AttackFireShort");
    MR::initActorState(this, _10, &NrvKoopaBattleMain::KoopaBattleMainNrvDamageEscape::sInstance, "DamageEscape");
    MR::initActorState(this, new KoopaStateJumpAway(mHost), &NrvKoopaBattleMain::KoopaBattleMainNrvJumpAway::sInstance, "JumpAway");

    if (KoopaFunction::isKoopaVs2(mHost) || KoopaFunction::isKoopaVs3(mHost)) {
        _28 = new KoopaStateAttackSpin(mHost);
        MR::initActorState(this, _28, &NrvKoopaBattleMain::KoopaBattleMainNrvAttackSpin::sInstance, "AttackSpin");

        _24 = new KoopaStateAttackRoll(mHost);
        MR::initActorState(this, _24, &NrvKoopaBattleMain::KoopaBattleMainNrvAttackRoll::sInstance, "AttackRoll");

        MR::initActorState(this, new KoopaStateAttackFireLong(mHost), &NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireLong::sInstance,
                           "AttackFireLong");
    }
}

void KoopaBattleMain::appear() {
    mIsDead = false;
    _2C = true;

    if (KoopaFunction::isKoopaVs1(mHost)) {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireShort::sInstance);
        }

        return;
    }

    if (KoopaFunction::isKoopaVs2(mHost)) {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireLong::sInstance);
        }

        return;
    }

    if (KoopaFunction::isKoopaVs3(mHost)) {
        if (KoopaFunction::isKoopaAngry(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireLong::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
        }
    }
}

void KoopaBattleMain::exeChasePlayer() {
    updateChasePlayer(sChasePlayerParam);

    if (KoopaFunction::isKoopaVs1(mHost)) {
        if (MR::isNearPlayer(mHost, 500.0f) ||
            ((MR::isGreaterStep(this, 60) && MR::isInSightFanPlayer(mHost, KoopaFunction::getKoopaFront(mHost), sAttackHipDropSight._0,
                                                                    sAttackHipDropSight._4, sAttackHipDropSight._8)))) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance);
            return;
        }
    }

    if (MR::isGreaterStep(this, 90)) {
        startMainAttack();
        return;
    }

    if (!KoopaFunction::isKoopaSightPlayer(mHost, sChasePlayerKeepSight)) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvWander::sInstance);
    }
}

void KoopaBattleMain::exeWander() {
    if (KoopaFunction::isKoopaSightPlayer(mHost, sChasePlayerStartSight)) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvFind::sInstance);
        return;
    }

    if (updateWander(sWanderParam)) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvSearch::sInstance);
    }
}

void KoopaBattleMain::exeSearch() {
    if (MR::isGreaterStep(this, 60)) {
        if (KoopaFunction::isKoopaVs1(mHost) && KoopaFunction::isKoopaSightPlayer(mHost, sAttackHipDropSight)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance);
            return;
        }

        if (KoopaFunction::isKoopaSightPlayer(mHost, sChasePlayerStartSight)) {
            startMainAttack();
            return;
        }

        if (MR::isNearPlayer(mHost, 2000.0f)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvFind::sInstance);
            return;
        }
    }

    if (updateSearch()) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
    }
}

void KoopaBattleMain::exeJumpAway() {
    if (MR::updateActorState(this)) {
        _2C = false;

        if (KoopaFunction::isKoopaVs3(mHost) && KoopaFunction::isKoopaLv3(mHost)) {
            _24->_1D = true;
        }

        startMainAttack();
    }
}

void KoopaBattleMain::exeGuard() {
    if (MR::updateActorState(this)) {
        if (KoopaFunction::isKoopaVs1(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance);
        } else if (KoopaFunction::isKoopaVs2(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackSpin::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackRoll::sInstance);
        }
    }
}

void KoopaBattleMain::exeAttackFireShort() {
    if (MR::updateActorState(this)) {
        if (KoopaFunction::isKoopaVs2(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackSpin::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
        }
    }
}

void KoopaBattleMain::exeAttackShockWave() {
    if (MR::updateActorState(this)) {
        if (KoopaFunction::isKoopaVs1(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvWander::sInstance);
        } else if (KoopaFunction::isKoopaVs2(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackSpin::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackRoll::sInstance);
        }
    }
}

void KoopaBattleMain::exeAttackHipDrop() {
    if (MR::updateActorState(this)) {
        if (MR::isPlayerDamaging()) {
            tryJumpAway();
        } else {
            startMainAttack();
        }

        return;
    }

    if (_1C->isDamage()) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvDamageEscape::sInstance);
    }
}

void KoopaBattleMain::exeAttackSpin() {
    if (MR::updateActorState(this)) {
        if (MR::isPlayerDamaging()) {
            tryJumpAway();
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance);
        }
    }
}

void KoopaBattleMain::exeAttackRoll() {
    if (MR::updateActorState(this)) {
        if (_24->_1C) {
            tryJumpAway();
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance);
        }
    }
}

void KoopaBattleMain::exeDamageEscape() {
    MR::updateActorStateAndNextNerve(this, &NrvKoopaBattleMain::KoopaBattleMainNrvSearch::sInstance);

    if (_10->isDownEnd()) {
        kill();
    }
}

bool KoopaBattleMain::tryCalcAndSetBaseMtx() {
    if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackRoll::sInstance)) {
        return _24->tryCalcAndSetBaseMtx();
    }

    return false;
}

bool KoopaBattleMain::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance)) {
        return _1C->attackSensor(pSender, pReceiver);
    }

    if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance)) {
        return _20->attackSensor(pSender, pReceiver);
    }

    if (!isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvJumpAway::sInstance) || !KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
        if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackRoll::sInstance)) {
            if (_24->attackSensor(pSender, pReceiver)) {
                return;
            }

            return KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver);
        }

        if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackSpin::sInstance)) {
            return _28->attackSensor(pSender, pReceiver);
        }

        if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvDamageEscape::sInstance)) {
            return _10->attackSensor(pSender, pReceiver);
        }

        if (!KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver) && KoopaFunction::tryKoopaBodyAttackPlayer(pSender, pReceiver)) {
            return;
        }
    }
}

bool KoopaBattleMain::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvDamageEscape::sInstance)) {
        return _10->tryDamage(msg, pSender, pReceiver);
    }

    if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackSpin::sInstance)) {
        if (_28->tryDamage(msg, pSender, pReceiver)) {
            if (_28->mIsDead) {
                setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance);
            }

            return true;
        }

        return false;
    }

    if (((isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvChasePlayer::sInstance) ||
          isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvWander::sInstance) || isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvSearch::sInstance) ||
          isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvFind::sInstance) || isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvJumpAway::sInstance) ||
          isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireShort::sInstance) ||
          isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireLong::sInstance)) ||
         (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackRoll::sInstance) && _24->isEnableGuard()) ||
         (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance) && _1C->isEnableGuard()) ||
         isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance)) &&
        _14->tryStart(msg, pSender, pReceiver)) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvGuard::sInstance);
        return true;
    }

    if (KoopaFunction::tryKoopaReflectStarPiece(msg, pSender, pReceiver)) {
        return true;
    }

    return false;
}

bool KoopaBattleMain::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_KOOPA_PLAYER_DAMAGE) {
        if (isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackRoll::sInstance)) {
            return true;
        }

        if (!isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackHipDrop::sInstance) &&
            !isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackSpin::sInstance) &&
            !isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance) &&
            !isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvJumpAway::sInstance) &&
            !isNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvDamageEscape::sInstance)) {
            tryJumpAway();
        }

        return true;
    }

    return false;
}

void KoopaBattleMain::startMainAttack() {
    if (_2C == false) {
        _2C = true;

        if (KoopaFunction::isKoopaVs2(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackSpin::sInstance);
        } else if (KoopaFunction::isKoopaVs3(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackRoll::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
        }
    } else if (KoopaFunction::isKoopaVs1(mHost)) {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireShort::sInstance);
        }
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireShort::sInstance);
        } else {
            setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireLong::sInstance);
        }
    } else if (KoopaFunction::isKoopaAngry(mHost)) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackFireLong::sInstance);
    } else {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
    }
}

bool KoopaBattleMain::tryJumpAway() {
    if (MR::isNearPlayer(mHost, 1500.0f)) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvJumpAway::sInstance);
        return true;
    } else {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvChasePlayer::sInstance);
        return false;
    }
}

void KoopaBattleMain::exeFind() {
    if (updateFind()) {
        setNerve(&NrvKoopaBattleMain::KoopaBattleMainNrvChasePlayer::sInstance);
    }
}

void KoopaBattleMain::exeAttackFireLong() {
    MR::updateActorStateAndNextNerve(this, &NrvKoopaBattleMain::KoopaBattleMainNrvAttackShockWave::sInstance);
}
