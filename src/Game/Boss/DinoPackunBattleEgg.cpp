#include "Game/Boss/DinoPackunBattleEgg.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunStateDamage.hpp"
#include "Game/Boss/DinoPackunTail.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace {
    static TVec3f sEggOutPosition = TVec3f(0.0f, 60.0f, -320.0f);
};

namespace NrvDinoPackunBattleEgg {
    NEW_NERVE(DinoPackunBattleEggNrvTurn, DinoPackunBattleEgg, Turn);
    NEW_NERVE(DinoPackunBattleEggNrvWalk, DinoPackunBattleEgg, Walk);
    NEW_NERVE(DinoPackunBattleEggNrvDamage, DinoPackunBattleEgg, Damage);
};  // namespace NrvDinoPackunBattleEgg

DinoPackunBattleEgg::DinoPackunBattleEgg(DinoPackun* pParent) : DinoPackunAction("1回戦卵バトル", pParent) {
    mStateDamage = nullptr;
    _18 = 1.0f;
}

void DinoPackunBattleEgg::init() {
    initNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvTurn::sInstance);
    mStateDamage = new DinoPackunStateDamage(getHost());
    mStateDamage->setDamageEgg();
}

void DinoPackunBattleEgg::appear() {
    mIsDead = false;
    getHost()->mTail->lockNodePosition(1);
    setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvTurn::sInstance);
}

void DinoPackunBattleEgg::attackSensor(HitSensor* a1, HitSensor* a2) {
    if (getHost()->isSensorEgg(a1)) {
        if (MR::isSensorPlayer(a2)) {
            MR::sendMsgPush(a2, a1);
        }
    }
}

bool DinoPackunBattleEgg::receiveMsgPlayerAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (!getHost()->isSensorEgg(a3)) {
        return false;
    }

    MR::isMsgPlayerSpinAttack(msg);
    return MR::isMsgStarPieceReflect(msg);
}

// an attempt was made
bool DinoPackunBattleEgg::receiveMsgPush(HitSensor* a2, HitSensor* a3) {
    if (!getHost()->isSensorEgg(a3)) {
        return false;
    }

    if (!isNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvWalk::sInstance) &&
        !isNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvTurn::sInstance) && getHost()->isSensorEgg(a2) && MR::isSensorMapObj(a3)) {
        TVec3f v11;
        MR::calcSensorHorizonNormalize(&v11, getHost()->mGravity, a3, a2);

        if (getHost()->mVelocity.dot(v11) < 0.0f) {
            TVec3f* vel = &getHost()->mVelocity;
            f32 v8 = v11.dot(*vel);
            JMAVECScaleAdd(&v11, vel, vel, -v8);
        }

        if (isNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvWalk::sInstance) && getHost()->_E8.dot(v11) < 0.0f) {
            TVec3f v10;
            MR::calcSideVec(&v10, getHost());

            f32 v9;

            if (v10.dot(v11) > 0.0f) {
                v9 = 1.0f;
            } else {
                v9 = -1.0f;
            }

            _18 = v9;
            setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvTurn::sInstance);
        }

        return true;
    }

    return false;
}

bool DinoPackunBattleEgg::receiveOtherMsg(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (isNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvDamage::sInstance)) {
        return mStateDamage->receiveOtherMsg(msg, a2, a3);
    }

    if (mStateDamage->isDamageMessage(msg)) {
        setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvDamage::sInstance);
        return true;
    }

    return false;
}

void DinoPackunBattleEgg::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "EggWalk", nullptr);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_WALK", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    getHost()->adjustTailRootPosition(sEggOutPosition, 1.0f);

    if (updateTurn(120, 0.30f)) {
        setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvWalk::sInstance);
    }
}

void DinoPackunBattleEgg::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "EggWalk", nullptr);
    }

    getHost()->adjustTailRootPosition(sEggOutPosition, 1.0f);

    if (updateWalk(300, 0.5f, 91)) {
        setNerve(&NrvDinoPackunBattleEgg::DinoPackunBattleEggNrvTurn::sInstance);
    }
}

DinoPackunBattleEgg::~DinoPackunBattleEgg() {
}

void DinoPackunBattleEgg::exeDamage() {
    if (MR::updateActorState(this, mStateDamage)) {
        kill();
    }
}
