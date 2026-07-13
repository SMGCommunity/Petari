#include "Game/Enemy/TakoHeiInk.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    static const f32 sModelSize = 50.0f;
    static const f32 sNormalFric = 0.99f;
    static const f32 sGravityAccel = 0.8f;
    static const s32 sBindOnTime = 10;
    static const s32 sForceBreakTime = 240;
};  // namespace

namespace NrvTakoHeiInk {
    NEW_NERVE(TakoHeiInkNrvWait, TakoHeiInk, Wait);
}; // namespace NrvTakoHeiInk

TakoHeiInk::TakoHeiInk(const char* pName) : LiveActor(pName) {
}

void TakoHeiInk::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TakoHeiInk", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::onCalcGravity(this);
    initHitSensor(1);

    MR::addHitSensorEnemy(this, "Body", 8, ::sModelSize, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(::sModelSize, 0.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(1, false);
    MR::initShadowVolumeSphere(this, ::sModelSize);
    initNerve(&NrvTakoHeiInk::TakoHeiInkNrvWait::sInstance);
    MR::invalidateClipping(this);
    makeActorDead();
}

void TakoHeiInk::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Break");
}

void TakoHeiInk::control() {
}

void TakoHeiInk::start(const TVec3f& startPos, const TVec3f& startVelocity, bool startOffBind) {
    mPosition.set(startPos);
    setNerve(&NrvTakoHeiInk::TakoHeiInkNrvWait::sInstance);
    appear();
    mVelocity.set(startVelocity);

    if (startOffBind) {
        MR::offBind(this);
    } else {
        MR::onBind(this);
    }
}

void TakoHeiInk::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        MR::startSound(this, "SE_EM_TAKOHEI_INK_BREAK");
        kill();
    }
}

void TakoHeiInk::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
    }

    if (MR::isGreaterStep(this, ::sBindOnTime) && MR::isNoBind(this)) {
        MR::onBind(this);
    }

    MR::addVelocityToGravity(this, ::sGravityAccel);
    MR::attenuateVelocity(this, ::sNormalFric);

    if (MR::isBinded(this) || MR::isGreaterStep(this, ::sForceBreakTime)) {
        MR::startSound(this, "SE_EM_TAKOHEI_INK_BREAK");
        kill();
    }
}
