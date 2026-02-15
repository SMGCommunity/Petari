#include "Game/Boss/DinoPackunBattleEggVs2.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunStateDamage.hpp"
#include "Game/Boss/DinoPackunStateFire.hpp"
#include "Game/Boss/DinoPackunTail.hpp"
#include "Game/Boss/DinoPackunTrackFire.hpp"
#include "Game/LiveActor/HitSensor.hpp"

namespace {
    static TVec3f sEggOutPosition = TVec3f(0.0f, 60.0f, -320.0f);
};

namespace NrvDinoPackunBattleEgg {
    NEW_NERVE(DinoPackunBattleEggVs2NrvTurn, DinoPackunBattleEggVs2, Turn);
    NEW_NERVE(DinoPackunBattleEggVs2NrvWalk, DinoPackunBattleEggVs2, Walk);
    NEW_NERVE(DinoPackunBattleEggVs2NrvDamage, DinoPackunBattleEggVs2, Damage);
};  // namespace NrvDinoPackunBattleEgg

DinoPackunBattleEggVs2::DinoPackunBattleEggVs2(DinoPackun* pPackun) : DinoPackunAction("2回戦卵バトル", pPackun) {
    mStateDamage = nullptr;
    mTrackFireHolder = nullptr;
    mStateFire = nullptr;
    _20 = 1.0f;
}

void DinoPackunBattleEggVs2::setTrackFire(DinoPackunTrackFireHolder* pHolder) {
    mTrackFireHolder = pHolder;
}

void DinoPackunBattleEggVs2::init() {
    initNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggVs2NrvTurn::sInstance);
    mStateDamage = new DinoPackunStateDamage(getHost());
    mStateDamage->setDamageEgg();
    mStateFire = new DinoPackunStateFire(getHost());
    mStateFire->init();
}

void DinoPackunBattleEggVs2::appear() {
    mIsDead = false;
    getHost()->mTail->lockNodePosition(1);
    mStateFire->requestFireDirect();
    setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggVs2NrvTurn::sInstance);
}

void DinoPackunBattleEggVs2::control() {
    mStateFire->update();
}

void DinoPackunBattleEggVs2::attackSensor(HitSensor* a1, HitSensor* a2) {
    if (getHost()->isSensorEgg(a1)) {
        if (MR::isSensorPlayer(a2)) {
            if (!MR::sendMsgEnemyAttackFire(a2, a1)) {
                if (MR::sendMsgPush(a2, a1)) {
                    return;
                }
            }
        } else if (!MR::sendMsgEnemyAttack(a2, a1)) {
            if (MR::sendMsgPush(a2, a1)) {
                return;
            }
        }
    }
}

bool DinoPackunBattleEggVs2::receiveMsgPlayerAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
    return (!getHost()->isSensorEgg(a3) ? false : MR::isMsgStarPieceReflect(msg));
}

bool DinoPackunBattleEggVs2::receiveMsgPush(HitSensor* a1, HitSensor* a2) {
    return getHost()->isSensorEgg(a2);
}

bool DinoPackunBattleEggVs2::receiveOtherMsg(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (isNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggVs2NrvDamage::sInstance)) {
        if (mStateDamage->receiveOtherMsg(msg, a2, a3)) {
            mStateFire->requestCool();
            return true;
        }
    } else if (mStateDamage->isDamageMessage(msg)) {
        mTrackFireHolder->killAll();
        setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggVs2NrvDamage::sInstance);
        return true;
    }

    return false;
}

void DinoPackunBattleEggVs2::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "EggWalk", nullptr);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_WALK", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_LAVER", -1, -1);
    }

    getHost()->adjustTailRootPosition(sEggOutPosition, 1.0f);

    if (updateTurn(30, 1.5f)) {
        setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggVs2NrvWalk::sInstance);
    }
}

void DinoPackunBattleEggVs2::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "EggWalk", nullptr);
        getHost()->mTail->_C = 1.5f;
    }

    getHost()->adjustTailRootPosition(sEggOutPosition, 1.0f);

    s32 step = getNerveStep();
    f32 v3;

    s32 div = (step / 100);
    if (div % 2 != 0) {
        v3 = 1.0f;
    } else {
        v3 = -1.0f;
    }

    MR::rotateDirectionGravityDegree(getHost(), &getHost()->_E8, (0.75f * v3));
    s32 v4 = getNerveStep() % 200;
    if (v4 < 80) {
        mStateFire->requestFire();
    } else {
        if (v4 >= 180) {
            mStateFire->requestFireSign();
        } else {
            mStateFire->requestCool();
        }
    }

    if (updateWalk(750, 0.89f, 40)) {
        setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggVs2NrvTurn::sInstance);
    }
}

void DinoPackunBattleEggVs2::exeDamage() {
    if (MR::isFirstStep(this)) {
        getHost()->mTail->_C = 1.0f;
    }

    if (MR::updateActorState(this, mStateDamage)) {
        kill();
    }
}

DinoPackunBattleEggVs2::~DinoPackunBattleEggVs2() {
    return;
}
