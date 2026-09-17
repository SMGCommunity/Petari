#include "Game/Boss/KoopaBattleMain.hpp"
#include "Game/Boss/Koopa.hpp"
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
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    static MR::ActorMoveParam sChasePlayerParam = {1.5f, 1.0f, 0.9f, 3.0f};
    static MR::ActorMoveParam sWanderParam = {0.5f, 1.0f, 0.9f, 1.3f};
    static MR::ActorSightParam sChasePlayerStartSight = {1800.0f, 120.0f, 50.0f};
    static MR::ActorSightParam sChasePlayerKeepSight = {2200.0f, 125.0f, 50.0f};
    static MR::ActorSightParam sAttackHipDropSight = {1000.0f, 90.0f, 30.0f};
    static const f32 sDistanceMaxToJumpAway = 1500.0f;
    static const f32 sSearchDistance = 2000.0f;
    static const s32 sSearchStepToFind = 60;
    static const f32 sChasePlayerDistanceNearAttack = 500.0f;
    static const s32 sChasePlayerStepToAttackFireShort = 90;
    static const s32 sChasePlayerStepToAttackHipDrop = 60;
};  // namespace

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
};  // namespace NrvKoopaBattleMain

KoopaBattleMain::KoopaBattleMain(const char* pName, Koopa* pKoopa)
    : KoopaBattleBase(pName, pKoopa), mStateAttackHipDrop(), mStateAttackShockWave(), mStateAttackRoll(), mStateAttackSpin(), _2C(true) {
}

void KoopaBattleMain::init() {
    KoopaBattleBase::init();
    initNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvWander));
    MR::initActorStateKeeper(this, 16);

    mStateAttackHipDrop = new KoopaStateAttackHipDrop(mHost);
    mStateAttackShockWave = new KoopaStateAttackShockWave(mHost);

    MR::initActorState(this, mStateGuard, GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvGuard), "Guard");
    MR::initActorState(this, mStateAttackHipDrop, GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop), "AttackHipDrop");
    MR::initActorState(this, mStateAttackShockWave, GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave), "AttackShockWave");
    MR::initActorState(this, new KoopaStateAttackFireShort(mHost), GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireShort), "AttackFireShort");
    MR::initActorState(this, mStateDamageEscape, GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvDamageEscape), "DamageEscape");
    MR::initActorState(this, new KoopaStateJumpAway(mHost), GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvJumpAway), "JumpAway");

    if (KoopaFunction::isKoopaVs2(mHost) || KoopaFunction::isKoopaVs3(mHost)) {
        mStateAttackSpin = new KoopaStateAttackSpin(mHost);
        MR::initActorState(this, mStateAttackSpin, GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackSpin), "AttackSpin");

        mStateAttackRoll = new KoopaStateAttackRoll(mHost);
        MR::initActorState(this, mStateAttackRoll, GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackRoll), "AttackRoll");

        MR::initActorState(this, new KoopaStateAttackFireLong(mHost), GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireLong), "AttackFireLong");
    }
}

void KoopaBattleMain::appear() {
    mIsDead = false;
    _2C = true;

    if (KoopaFunction::isKoopaVs1(mHost)) {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
        } else {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireShort));
        }

        return;
    }

    if (KoopaFunction::isKoopaVs2(mHost)) {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
        } else {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireLong));
        }

        return;
    }

    if (KoopaFunction::isKoopaVs3(mHost)) {
        if (KoopaFunction::isKoopaAngry(mHost)) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireLong));
        } else {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
        }
    }
}

void KoopaBattleMain::exeChasePlayer() {
    updateChasePlayer(::sChasePlayerParam);

    if (KoopaFunction::isKoopaVs1(mHost)) {
        if (MR::isNearPlayer(mHost, ::sChasePlayerDistanceNearAttack) ||
            ((MR::isGreaterStep(this, ::sChasePlayerStepToAttackHipDrop) &&
              MR::isInSightFanPlayer(mHost, KoopaFunction::getKoopaFront(mHost), ::sAttackHipDropSight._0, ::sAttackHipDropSight._4,
                                     ::sAttackHipDropSight._8)))) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop));
            return;
        }
    }

    if (MR::isGreaterStep(this, ::sChasePlayerStepToAttackFireShort)) {
        startMainAttack();
        return;
    }

    if (!KoopaFunction::isKoopaSightPlayer(mHost, ::sChasePlayerKeepSight)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvWander));
    }
}

void KoopaBattleMain::exeWander() {
    if (KoopaFunction::isKoopaSightPlayer(mHost, ::sChasePlayerStartSight)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvFind));
        return;
    }

    if (updateWander(::sWanderParam)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvSearch));
    }
}

void KoopaBattleMain::exeSearch() {
    if (MR::isGreaterStep(this, ::sSearchStepToFind)) {
        if (KoopaFunction::isKoopaVs1(mHost) && KoopaFunction::isKoopaSightPlayer(mHost, ::sAttackHipDropSight)) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop));
            return;
        }

        if (KoopaFunction::isKoopaSightPlayer(mHost, ::sChasePlayerStartSight)) {
            startMainAttack();
            return;
        }

        if (MR::isNearPlayer(mHost, ::sSearchDistance)) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvFind));
            return;
        }
    }

    if (updateSearch()) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
    }
}

void KoopaBattleMain::exeJumpAway() {
    if (!MR::updateActorState(this)) {
        return;
    }

    _2C = false;

    if (KoopaFunction::isKoopaVs3(mHost) && KoopaFunction::isKoopaLv3(mHost)) {
        mStateAttackRoll->_1D = true;
    }

    startMainAttack();
}

void KoopaBattleMain::exeGuard() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (KoopaFunction::isKoopaVs1(mHost)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop));
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackSpin));
    } else {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackRoll));
    }
}

void KoopaBattleMain::exeAttackFireShort() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (KoopaFunction::isKoopaVs2(mHost)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackSpin));
    } else {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
    }
}

void KoopaBattleMain::exeAttackShockWave() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (KoopaFunction::isKoopaVs1(mHost)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvWander));
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackSpin));
    } else {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackRoll));
    }
}

void KoopaBattleMain::exeAttackHipDrop() {
    if (MR::updateActorState(this)) {
        if (MR::isPlayerDamaging()) {
            tryJumpAway();
        } else {
            startMainAttack();
        }
    } else if (mStateAttackHipDrop->isDamage()) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvDamageEscape));
    }
}

void KoopaBattleMain::exeAttackSpin() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (MR::isPlayerDamaging()) {
        tryJumpAway();
    } else {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop));
    }
}

void KoopaBattleMain::exeAttackRoll() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (mStateAttackRoll->_1C) {
        tryJumpAway();
    } else {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop));
    }
}

void KoopaBattleMain::exeDamageEscape() {
    MR::updateActorStateAndNextNerve(this, GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvSearch));

    if (mStateDamageEscape->isDownEnd()) {
        kill();
    }
}

bool KoopaBattleMain::tryCalcAndSetBaseMtx() {
    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackRoll))) {
        return mStateAttackRoll->tryCalcAndSetBaseMtx();
    }

    return false;
}

void KoopaBattleMain::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop))) {
        mStateAttackHipDrop->attackSensor(pSender, pReceiver);
        return;
    }

    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave))) {
        mStateAttackShockWave->attackSensor(pSender, pReceiver);
        return;
    }

    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvJumpAway)) && KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
        return;
    }

    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackRoll))) {
        if (mStateAttackRoll->attackSensor(pSender, pReceiver)) {
            return;
        }

        KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver);
        return;
    }

    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackSpin))) {
        mStateAttackSpin->attackSensor(pSender, pReceiver);
        return;
    }

    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvDamageEscape))) {
        mStateDamageEscape->attackSensor(pSender, pReceiver);
        return;
    }

    if (!KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver) && KoopaFunction::tryKoopaBodyAttackPlayer(pSender, pReceiver)) {
        return;
    }
}

bool KoopaBattleMain::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvDamageEscape))) {
        return mStateDamageEscape->tryDamage(msg, pSender, pReceiver);
    }

    if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackSpin))) {
        if (mStateAttackSpin->tryDamage(msg, pSender, pReceiver)) {
            if (mStateAttackSpin->mIsDead) {
                setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop));
            }

            return true;
        }

        return false;
    }

    if (((isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvChasePlayer)) || isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvWander)) ||
          isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvSearch)) || isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvFind)) ||
          isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvJumpAway)) || isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireShort)) ||
          isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireLong))) ||
         (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackRoll)) && mStateAttackRoll->isEnableGuard()) ||
         (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop)) && mStateAttackHipDrop->isEnableGuard()) ||
         isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave))) &&
        mStateGuard->tryStart(msg, pSender, pReceiver)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvGuard));
        return true;
    }

    if (KoopaFunction::tryKoopaReflectStarPiece(msg, pSender, pReceiver)) {
        return true;
    }

    return false;
}

bool KoopaBattleMain::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_KOOPA_PLAYER_DAMAGE) {
        if (isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackRoll))) {
            return true;
        }

        if (!isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackHipDrop)) &&
            !isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackSpin)) &&
            !isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave)) &&
            !isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvJumpAway)) &&
            !isNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvDamageEscape))) {
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
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackSpin));
        } else if (KoopaFunction::isKoopaVs3(mHost)) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackRoll));
        } else {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
        }
    } else if (KoopaFunction::isKoopaVs1(mHost)) {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
        } else {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireShort));
        }
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        if (KoopaFunction::isKoopaLv1(mHost)) {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireShort));
        } else {
            setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireLong));
        }
    } else if (KoopaFunction::isKoopaAngry(mHost)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackFireLong));
    } else {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
    }
}

bool KoopaBattleMain::tryJumpAway() {
    if (MR::isNearPlayer(mHost, ::sDistanceMaxToJumpAway)) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvJumpAway));
        return true;
    }

    setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvChasePlayer));
    return false;
}

void KoopaBattleMain::exeFind() {
    if (updateFind()) {
        setNerve(GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvChasePlayer));
    }
}

void KoopaBattleMain::exeAttackFireLong() {
    MR::updateActorStateAndNextNerve(this, GET_NERVE(KoopaBattleMain, KoopaBattleMainNrvAttackShockWave));
}

KoopaBattleMain::~KoopaBattleMain() {
}
