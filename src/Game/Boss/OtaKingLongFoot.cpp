#include "Game/Boss/OtaKingLongFoot.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"

namespace NrvOtaKingLongFoot {
    NEW_NERVE(OtaKingLongFootNrvWait, OtaKingLongFoot, Wait)
    NEW_NERVE(OtaKingLongFootNrvDamage, OtaKingLongFoot, Damage)
    NEW_NERVE(OtaKingLongFootNrvAppearDemo, OtaKingLongFoot, AppearDemo)
    NEW_NERVE(OtaKingLongFootNrvDownDemo, OtaKingLongFoot, DownDemo)
};  // namespace NrvOtaKingLongFoot

namespace {
    const f32 cFoot14SensorRadius = 50.0f;
    const Vec cFoot14SensorOffset = {50.0f, 0.0f, 20.0f};
    const f32 cFoot12SensorRadius = 90.0f;
    const Vec cFoot12SensorOffset = {75.0f, 0.0f, 20.0f};
    const f32 cFoot10SensorRadius = 110.0f;
    const Vec cFoot10SensorOffset = {125.0f, 0.0f, 0.0f};
    const f32 cFoot08SensorRadius = 150.0f;
    const Vec cFoot08SensorOffset = {125.0f, 0.0f, -40.0f};
    const f32 cFoot06SensorRadius = 150.0f;
    const Vec cFoot06SensorOffset = {100.0f, 0.0f, -30.0f};
    const f32 cFoot05SensorRadius = 150.0f;
    const Vec cFoot05SensorOffset = {80.0f, 0.0f, -40.0f};
};  // namespace

OtaKingLongFoot::OtaKingLongFoot(LiveActor* pHost, s32 demoBckStep, const char* pName)
    : PartsModel(pHost, pName, "OtaKingLongFoot", nullptr, MR::DrawBufferType_Enemy, false), mDemoBckStep(demoBckStep), mScaleController(nullptr) {
}

void OtaKingLongFoot::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::initLightCtrl(this);
    initSensor();
    mScaleController = new AnimScaleController(nullptr);
    initNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance);
    PartsModel::init(rIter);
}

void OtaKingLongFoot::hide() {
    MR::startBckNoInterpole(this, "Appear");
    MR::setBckFrameAndStop(this, 0.0f);
    MR::hideModel(this);
}

void OtaKingLongFoot::startAppearDemo() {
    setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvAppearDemo::sInstance);
}

void OtaKingLongFoot::startDownDemo() {
    setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvDownDemo::sInstance);
}

void OtaKingLongFoot::endDemo() {
    setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance);
}

void OtaKingLongFoot::control() {
    mScaleController->updateNerve();
}

void OtaKingLongFoot::calcAndSetBaseMtx() {
    PartsModel::calcAndSetBaseMtx();
    TVec3f scale = mScaleController->_C * mScale;
    MR::setBaseScale(this, scale);
}

void OtaKingLongFoot::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (isNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
            setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvDamage::sInstance);
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool OtaKingLongFoot::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        mScaleController->startHitReaction();
        return true;
    }
    return false;
}

void OtaKingLongFoot::initSensor() {
    initHitSensor(6);
    MR::addHitSensorAtJointEnemy(this, "Foot14", "Foot14", 8, ::cFoot14SensorRadius, TVec3f(::cFoot14SensorOffset));
    MR::addHitSensorAtJointEnemy(this, "Foot12", "Foot12", 8, ::cFoot12SensorRadius, TVec3f(::cFoot12SensorOffset));
    MR::addHitSensorAtJointEnemy(this, "Foot10", "Foot10", 8, ::cFoot10SensorRadius, TVec3f(::cFoot10SensorOffset));
    MR::addHitSensorAtJointEnemy(this, "Foot08", "Foot08", 8, ::cFoot08SensorRadius, TVec3f(::cFoot08SensorOffset));
    MR::addHitSensorAtJointEnemy(this, "Foot06", "Foot06", 8, ::cFoot06SensorRadius, TVec3f(::cFoot06SensorOffset));
    MR::addHitSensorAtJointEnemy(this, "Foot05", "Foot05", 8, ::cFoot05SensorRadius, TVec3f(::cFoot05SensorOffset));
}

bool OtaKingLongFoot::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance)) {
        setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvDamage::sInstance);
    }
    return true;
}

void OtaKingLongFoot::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::setBckFrameAtRandom(this);
    }
}

void OtaKingLongFoot::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "Damage");
    }

    MR::setNerveAtBckStopped(this, &NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance);
}

void OtaKingLongFoot::exeAppearDemo() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "Appear");
        MR::setBckFrameAndStop(this, 0.0f);
        MR::showModel(this);
    }

    if (MR::isStep(this, mDemoBckStep)) {
        MR::setBckRate(this, 1.0f);
    }
}

void OtaKingLongFoot::exeDownDemo() {
    if (MR::isStep(this, mDemoBckStep)) {
        MR::startBckNoInterpole(this, "Down");
    }
}
