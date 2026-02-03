#include "Game/MapObj/ManholeCover.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvManholeCover {
    NEW_NERVE(HostTypeWait, ManholeCover, Wait);
    NEW_NERVE(HostTypeOpen, ManholeCover, Open);
    NEW_NERVE(HostTypeRattle, ManholeCover, Rattle);
};  // namespace NrvManholeCover

ManholeCover::ManholeCover(const char* pName) : MapObjActor(pName) {
}

void ManholeCover::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(0);
    info.setupSound(4);
    info.setupHitSensor();
    TVec3f param;
    param.x = 0.0f;
    param.y = 0.0f;
    param.z = 0.0f;
    info.setupHitSensorParam(1, 150.0f, param);
    info.setupNerve(&NrvManholeCover::HostTypeWait::sInstance);
    MapObjActor::initialize(rIter, info);
}

bool ManholeCover::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if ((MR::isMsgPlayerHipDropFloor(msg) || MR::isMsgPlayerUpperPunch(msg)) &&
        (isNerve(&NrvManholeCover::HostTypeWait::sInstance) || isNerve(&NrvManholeCover::HostTypeRattle::sInstance))) {
        setNerve(&NrvManholeCover::HostTypeOpen::sInstance);
        return true;
    }

    return false;
}

bool ManholeCover::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgSpinStormRange(msg) && isNerve(&NrvManholeCover::HostTypeWait::sInstance)) {
        setNerve(&NrvManholeCover::HostTypeRattle::sInstance);
        return true;
    }
    return false;
}

void ManholeCover::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Rotation", nullptr);
        MR::invalidateCollisionParts(this);
    }

    if (MR::isBckStopped(this)) {
        MR::validateCollisionParts(this);
        setNerve(&NrvManholeCover::HostTypeWait::sInstance);
    }
}

void ManholeCover::exeRattle() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Vibration", nullptr);
        MR::startSound(this, "SE_OJ_MANHOLE_RATTLE", -1, -1);
    }

    if (MR::isGreaterStep(this, 45) && MR::isBckStopped(this)) {
        setNerve(&NrvManholeCover::HostTypeWait::sInstance);
    }
}

void ManholeCover::exeWait() {
}
