#include "Game/MapObj/ScrewSwitch.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void ScrewSwitch_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)3.0f;
}

namespace {
    static const s32 sStepForAdjust = 3;
    static const s32 sStepToRumbleCamera = 30;
    static const s32 sStepForScrewStart = 30;
    static const s32 sStepForPause = 0;
}  // namespace

namespace NrvScrewSwitch {
    NEW_NERVE(ScrewSwitchNrvWait, ScrewSwitch, Wait);
    NEW_NERVE(ScrewSwitchNrvAdjust, ScrewSwitch, Adjust);
    NEW_NERVE(ScrewSwitchNrvScrew, ScrewSwitch, Screw);
    NEW_NERVE(ScrewSwitchNrvEnd, ScrewSwitch, End);
};  // namespace NrvScrewSwitch

ScrewSwitch::ScrewSwitch(const char* pName) : LiveActor(pName) {
    mCollisionParts = nullptr;
    _90 = nullptr;
    mMapObjConnector = nullptr;
    mMapObjConnector = new MapObjConnector(this);
}

void ScrewSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::needStageSwitchWriteA(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    initModelManagerWithAnm("ScrewSwitch", nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addBodyMessageSensorMapObj(this);
    TVec3f offset;
    offset.scale(-150.0f, mGravity);
    MR::addHitSensorAtJoint(this, "binder", "Screw", ATYPE_BINDER, 8, 150.0f, offset);
    MtxPtr jointMtx = MR::getJointMtx(this, "Screw");
    HitSensor* Sensor = getSensor("binder");
    MR::initCollisionParts(this, "ScrewCol", Sensor, jointMtx);
    mCollisionParts = MR::createCollisionPartsFromLiveActor(this, "ScrewReceiveCol", getSensor("body"), MR::getJointMtx(this, "ScrewReceive"),
                                                            MR::CollisionScaleType_NotUsingScale);
    MR::validateCollisionParts(mCollisionParts);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::tryRegisterDemoCast(this, rIter);
    initNerve(&NrvScrewSwitch::ScrewSwitchNrvWait::sInstance);
    makeActorAppeared();
}

void ScrewSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "ScrewSwitchOn");
        MR::setBrkFrameAndStop(this, 0.0f);
    }
}

void ScrewSwitch::exeAdjust() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "ScrewSwitchOn");
        MR::setBrkFrameAndStop(this, 0.0f);
    }

    _90->mPosition.lerp(_90->mPosition, getSensor("binder")->mPosition, static_cast< f32 >(getNerveStep()) / ::sStepForAdjust);
    if (MR::isStep(this, ::sStepForAdjust)) {
        setNerve(&NrvScrewSwitch::ScrewSwitchNrvScrew::sInstance);
    }
}

void ScrewSwitch::exeScrew() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ScrewSwitchOn", nullptr);
        MR::startBrk(this, "ScrewSwitchOn");
        MR::startBckPlayer("ScrewSwitchOn", static_cast< const char* >(nullptr));
    }
    if (MR::isLessStep(this, ::sStepForScrewStart)) {
        MR::startLevelSound(this, "SE_OJ_LV_SCREW_SWITCH_MOVE");
    }
    if (MR::isStep(this, ::sStepToRumbleCamera)) {
        MR::startSound(this, "SE_OJ_SCREW_SWITCH_ON");
        MR::tryRumblePadVeryStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
    }
    if (MR::isBckStopped(this)) {
        MR::onSwitchA(this);
        setNerve(&NrvScrewSwitch::ScrewSwitchNrvEnd::sInstance);
    }
}

void ScrewSwitch::exeEnd() {
    if (MR::isStep(this, ::sStepForPause)) {
        MR::invalidateHitSensors(this);
        if (MR::isPlayerInRush()) {
            MR::endBindAndPlayerWait(this);
            _90 = nullptr;
        }
    }
}

void ScrewSwitch::initAfterPlacement() {
    mMapObjConnector->attachToUnder();
}

void ScrewSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mMapObjConnector->connect();
}

void ScrewSwitch::control() {
    mCollisionParts->setMtx();
}

bool ScrewSwitch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgRushBegin(msg) && MR::isSensorPlayer(pSender) && MR::isOnPlayer(getSensor("binder"))) {
        _90 = pSender->mHost;
        MR::startSound(_90, "SE_PV_TWIST_START");
        MR::startSound(_90, "SE_PM_SPIN_ATTACK");
        setNerve(&NrvScrewSwitch::ScrewSwitchNrvAdjust::sInstance);
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg) && _90 && isNerve(&NrvScrewSwitch::ScrewSwitchNrvScrew::sInstance)) {
        updateBindActorMtx();
        return true;
    }

    return false;
}

void ScrewSwitch::updateBindActorMtx() {
    TPos3f pos;
    pos.set(_90->getBaseMtx());
    pos.setTrans(getSensor("binder")->mPosition);
    MR::setBaseTRMtx(_90, pos);
}
