#include "Game/MapObj/ScrewSwitch.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
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
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvScrewSwitch {
    NEW_NERVE(ScrewSwitchNrvWait, ScrewSwitch, Wait);
    NEW_NERVE(ScrewSwitchNrvAdjust, ScrewSwitch, Adjust);
    NEW_NERVE(ScrewSwitchNrvScrew, ScrewSwitch, Screw);
    NEW_NERVE(ScrewSwitchNrvEnd, ScrewSwitch, End);
}

ScrewSwitch::ScrewSwitch(const char* pName) : LiveActor(pName) {
    mCollisionParts = nullptr;
    _90 = nullptr;
    mMapObjConnector = nullptr;
    mMapObjConnector = new MapObjConnector(this);
}

ScrewSwitch::~ScrewSwitch() {  }

void ScrewSwitch::init(const JMapInfoIter & rIter) {
    MR::initDefaultPos(this, rIter);
    MR::needStageSwitchWriteA(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    initModelManagerWithAnm("ScrewSwitch", nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addBodyMessageSensorMapObj(this);
    TVec3f var1;
    var1.scale(-150.0f, mGravity);
    MR::addHitSensorAtJoint(this, "binder", "Screw", 97, 8, 150.0f, var1);
    MtxPtr jointMtx = MR::getJointMtx(this, "Screw");
    HitSensor* Sensor = getSensor("binder");
    MR::initCollisionParts(this, "ScrewCol", Sensor, jointMtx);
    mCollisionParts = MR::createCollisionPartsFromLiveActor(this, "ScrewReceiveCol", getSensor("body"), MR::getJointMtx(this, "ScrewReceive"), (MR::CollisionScaleType)1);
    MR::validateCollisionParts(mCollisionParts);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::tryRegisterDemoCast(this, rIter);
    initNerve(&NrvScrewSwitch::ScrewSwitchNrvWait::sInstance);
    makeActorAppeared();
}

//Theres an Inline at JMAVECLearp. Scratch: https://decomp.me/scratch/LUQFL
void ScrewSwitch::exeAdjust() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "ScrewSwitchOn");
        MR::setBrkFrameAndStop(this, 0.0f);
    }
    f32 step = getNerveStep() / 3.0f;
    HitSensor* sensor = getSensor("body");
    JMAVECLerp((const Vec*)_90, &sensor->mPosition, &mPosition, step);
    if (MR::isStep(this, 3)) {
        setNerve(&NrvScrewSwitch::ScrewSwitchNrvScrew::sInstance);
    }
}

void ScrewSwitch::exeScrew() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ScrewSwitchOn", nullptr);
        MR::startBrk(this, "ScrewSwitchOn");
        MR::startBckPlayer("ScrewSwitchOn", (const char*)0);
    }
    if (MR::isLessStep(this, 30)) {
        MR::startLevelSound(this, "SE_OJ_LV_SCREW_SWITCH_MOVE", -1, -1, -1);
    }
    if (MR::isStep(this, 30)) {
        MR::startSound(this, "SE_OJ_SCREW_SWITCH_ON", -1, -1);
        MR::tryRumblePadVeryStrong(this, 0);
        MR::shakeCameraNormal();
    }
    if (MR::isBckStopped(this)) {
        MR::onSwitchA(this);
        setNerve(&NrvScrewSwitch::ScrewSwitchNrvEnd::sInstance);
    }
}

void ScrewSwitch::exeEnd() {
    if (MR::isStep(this, 0)) {
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

bool ScrewSwitch::receiveOtherMsg(u32 a0, HitSensor* a1 , HitSensor* a2) { 
    HitSensor* Sensor;
    if (MR::isMsgRushBegin(a0) && MR::isSensorPlayer(a1) && MR::isOnPlayer(getSensor("binder"))) {
        _90 = a1->mActor;
        MR::startSound(_90, "SE_PV_TWIST_START", -1, -1);
        MR::startSound(_90, "SE_PM_SPIN_ATTACK", -1, -1);
        setNerve(&NrvScrewSwitch::ScrewSwitchNrvAdjust::sInstance);
        return true;
    } else if (MR::isMsgUpdateBaseMtx(a0) && _90 && isNerve(&NrvScrewSwitch::ScrewSwitchNrvScrew::sInstance)) {
        updateBindActorMtx();
        return true;
    }
    return false;
}

void ScrewSwitch::updateBindActorMtx() {
    TPos3f pos;
    pos.setInline(_90->getBaseMtx());
    HitSensor* Sensor = getSensor("binder");
    pos.mMtx[0][3] = Sensor->mPosition.x;
    pos.mMtx[1][3] = Sensor->mPosition.y;
    pos.mMtx[2][3] = Sensor->mPosition.z;
    
    MR::setBaseTRMtx(_90, pos);
}

inline void ScrewSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "ScrewSwitchOn");
        MR::setBrkFrameAndStop(this, 0.0f);
    }
}
