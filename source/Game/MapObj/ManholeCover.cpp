#include "Game/MapObj/ManholeCover.hpp"

void ManholeCover::init(const JMapInfoIter &rIter) {
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

bool ManholeCover::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (!MR::isMsgPlayerHipDropFloor(msg) && !MR::isMsgPlayerUpperPunch(msg) || !isNerve(&NrvManholeCover::HostTypeWait::sInstance) && !isNerve(&NrvManholeCover::HostTypeRattle::sInstance)) {
        return false;
    }

    setNerve(&NrvManholeCover::HostTypeOpen::sInstance);
    return true;
}