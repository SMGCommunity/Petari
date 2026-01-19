#include "Game/Enemy/WaterBazookaCapsule.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvWaterBazookaCapsule {
    NEW_NERVE(WaterBazookaCapsuleNrvNormal, WaterBazookaCapsule, Normal);
    NEW_NERVE(WaterBazookaCapsuleNrvCrack, WaterBazookaCapsule, Crack);
    NEW_NERVE(WaterBazookaCapsuleNrvBreak, WaterBazookaCapsule, Break);
}  // namespace NrvWaterBazookaCapsule

WaterBazookaCapsule::WaterBazookaCapsule(LiveActor* pActor, const char* pName)
    : PartsModel(pActor, pName, "WaterBazookaCapsule", nullptr, MR::DrawBufferType_Enemy, false) {}

void WaterBazookaCapsule::init(const JMapInfoIter& rIter) {
    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_WATER_BAZOOKA_CAPSULE, 4, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));

    MR::initCollisionParts(this, "WaterBazookaCapsule", getSensor(nullptr), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::addToAttributeGroupSearchTurtle(this);

    MR::startBrk(this, "Damage");
    MR::setBrkFrameAndStop(this, 0.0f);
    initNerve(&NrvWaterBazookaCapsule::WaterBazookaCapsuleNrvNormal::sInstance);
    makeActorAppeared();
}

void WaterBazookaCapsule::crackCapsule() {
    setNerve(&NrvWaterBazookaCapsule::WaterBazookaCapsuleNrvCrack::sInstance);
}

void WaterBazookaCapsule::breakCapsule() {
    setNerve(&NrvWaterBazookaCapsule::WaterBazookaCapsuleNrvBreak::sInstance);
}

bool WaterBazookaCapsule::isPlayerOnCapsule() const {
    return MR::isOnPlayer(getSensor(nullptr));
}

void WaterBazookaCapsule::exeNormal() {
    if (MR::isFirstStep(this)) {
        MR::startBva(this, "Normal");
    }
}

void WaterBazookaCapsule::exeCrack() {
    if (MR::isFirstStep(this)) {
        MR::stopScene(12);
        MR::startBva(this, "Crack");
        MR::emitEffect(this, "Crack");
        MR::startSound(this, "SE_EM_WATERBAZ_CAPSULE_DAMAGE", -1, -1);
    }
}

void WaterBazookaCapsule::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::stopScene(12);
        MR::startBva(this, "Break");
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_EM_WATERBAZ_CAPSULE_BREAK", -1, -1);
        MR::invalidateCollisionParts(this);
    }
}

bool WaterBazookaCapsule::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvWaterBazookaCapsule::WaterBazookaCapsuleNrvBreak::sInstance)) {
        return false;
    }

    if (!MR::isEffectValid(this, "Crack")) {
        return mHost->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    return false;
}

bool WaterBazookaCapsule::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvWaterBazookaCapsule::WaterBazookaCapsuleNrvBreak::sInstance)) {
        return false;
    }

    if (!MR::isEffectValid(this, "Crack")) {
        return mHost->receiveMsgEnemyAttack(msg, pSender, pReceiver);
    }

    return false;
}

bool WaterBazookaCapsule::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvWaterBazookaCapsule::WaterBazookaCapsuleNrvBreak::sInstance)) {
        return false;
    }

    if (!MR::isEffectValid(this, "Crack")) {
        return mHost->receiveOtherMsg(msg, pSender, pReceiver);
    }

    return false;
}
