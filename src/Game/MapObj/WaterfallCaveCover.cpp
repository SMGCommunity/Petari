#include "Game/MapObj/WaterfallCaveCover.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sSensorRadius = 500.0f;
    static const s32 sRiddleSeStep = 90;
};  // namespace

namespace NrvWaterfallCaveCover {
    NEW_NERVE(HostTypeWait, WaterfallCaveCover, Wait);
    NEW_NERVE(HostTypeBreak, WaterfallCaveCover, Break);
};  // namespace NrvWaterfallCaveCover

void WaterfallCaveCover::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo rInitInfo;
    rInitInfo.setupHioNode("n形オブジェ");
    rInitInfo.setupDefaultPos();
    rInitInfo.setupConnectToScene();
    rInitInfo.setupSound(4);
    rInitInfo.setupGroupClipping(16);
    rInitInfo.setupNerve(&NrvWaterfallCaveCover::HostTypeWait::sInstance);
    rInitInfo.setupHitSensor();
    rInitInfo.setupHitSensorParam(8, ::sSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    rInitInfo.setupEffect(nullptr);
    initialize(rIter, rInitInfo);
}

bool WaterfallCaveCover::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgJetTurtleAttack(msg) && isNerve(&NrvWaterfallCaveCover::HostTypeWait::sInstance)) {
        setNerve(&NrvWaterfallCaveCover::HostTypeBreak::sInstance);
        return true;
    }
    return false;
}

void WaterfallCaveCover::exeWait() {
}

void WaterfallCaveCover::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "body");
        MR::invalidateCollisionParts(this);
        MapObjActorUtil::startBreak(this);
        MR::startRumbleWithShakeCameraStrong(this, "中", "弱", 1000.0f, FLOAT_MAX);
        MR::startSound(this, "SE_OJ_WATERFALL_COVER_BREAK");
    }

    if (MR::isStep(this, ::sRiddleSeStep)) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S");
    }

    if (MapObjActorUtil::isBreakStopped(this)) {
        MR::validateClipping(mModelObj);
        mModelObj->kill();
        kill();
    }
}
