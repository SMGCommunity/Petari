#include "Game/Enemy/KabokuriFire.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvKabokuriFire {
    NEW_NERVE(KabokuriFireNrvWait, KabokuriFire, Wait);
};

KabokuriFire::KabokuriFire(const char* pName) : LiveActor(pName) {
    _8C.identity();
}

void KabokuriFire::init(const JMapInfoIter& rIter) {
    MR::connectToSceneEnemyMovement(this);
    initNerve(&NrvKabokuriFire::KabokuriFireNrvWait::sInstance);
    initEffectKeeper(0, "KabokuriFire", false);
    initSound(2, false);
    initHitSensor(1);

    MR::addHitSensorEnemy(this, "body", 8, 90.0f, TVec3f(0.0f, 40.0f, 0.0f));
    MR::invalidateClipping(this);
    makeActorDead();
}

void KabokuriFire::start(const TVec3f& rPosition, bool gravitySet) {
    mPosition.set(rPosition);

    if (!gravitySet) {
        MR::calcGravity(this);
    }

    MR::makeMtxUpNoSupport(&_8C, -mGravity);
    _8C.setTrans(mPosition);
    setNerve(&NrvKabokuriFire::KabokuriFireNrvWait::sInstance);
    appear();
}

void KabokuriFire::control() { }

void KabokuriFire::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    MR::sendMsgEnemyAttackFire(pReceiver, pSender);
}

bool KabokuriFire::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isInSpinStormRange(msg, pSender, pReceiver, 350.0f)) {
        MR::startSound(this, "SE_OJ_PHANTOM_TOACH_OFF");
        kill();
        return true;
    }
    return false;
}

void KabokuriFire::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "KabokuriFire");
    }
    MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN");

    if (MR::isGreaterStep(this, 320)) {
        MR::deleteEffectAll(this);
        kill();
    }
}

KabokuriFire::~KabokuriFire() { }
