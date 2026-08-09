#include "Game/MapObj/HipDropTimerSwitch.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SpringValue.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace {
    static const char* sTimerSeSet[4] = {0, 0, 0, 0};
};

namespace NrvHipDropSwitch {
    NEW_NERVE(HipDropTimerSwitchNrvOff, HipDropTimerSwitch, Off);
    NEW_NERVE(HipDropTimerSwitchNrvSwitchDown, HipDropTimerSwitch, SwitchDown);
    NEW_NERVE(HipDropTimerSwitchNrvOn, HipDropTimerSwitch, On);
    NEW_NERVE(HipDropTimerSwitchNrvSwitchUp, HipDropTimerSwitch, SwitchUp);
};  // namespace NrvHipDropSwitch

HipDropTimerSwitch::HipDropTimerSwitch(const char* pName)
    : LiveActor(pName), _8C(0), _94(nullptr), _98(nullptr), mTimerMax(0x12C), _D0(0), _D5(0), _D6(0) {
    _90 = new SpringValue;
    _94 = new MapObjConnector(this);
    _90->setParam(0.0f, 0.0f, 0.2f, 0.9f, 0.0f);
    _9C.identity();
}

HipDropTimerSwitch::~HipDropTimerSwitch() {
}

void HipDropTimerSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HipDropTimerSwitch", nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    MR::initLightCtrl(this);
    initSound(4, false);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 16, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorMapObj(this, "hit", 16, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initStarPointerTarget(this, 150.0f, TVec3f(0.0f));
    MR::initCollisionParts(this, "body", getSensor("hit"), nullptr);
    _98 = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("Move"), MR::CollisionScaleType_Unk2);
    MR::validateCollisionParts(_98);
    initNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvOff::sInstance);
    MR::needStageSwitchWriteA(this, rIter);
    MR::tryRegisterDemoCast(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mTimerMax);
    MR::getJMapInfoArg1NoInit(rIter, &_D0);

    if (_D0 <= 0) {
        _D0 = 0;
    }

    initEffectKeeper(0, nullptr, false);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
        return;
    }

    makeActorAppeared();
}

void HipDropTimerSwitch::initAfterPlacement() {
    _94->attachToUnder();
}

void HipDropTimerSwitch::control() {
    _D5 = _D6;
    _D6 = 0;
}

void HipDropTimerSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    _94->connect();
}

void HipDropTimerSwitch::updateTimerSE() {
    if (MR::isPlayerDead()) {
        return;
    }

    if (MR::isPowerStarGetDemoActive()) {
        return;
    }

    s32 frame = mTimerMax % 0x3C;
    s32 nerveStep = getNerveStep() - frame;

    if (frame != 0) {
        nerveStep += 0x3C;
    }

    if (nerveStep < 0x3C) {
        return;
    }

    frame = mTimerMax;

    if (mTimerMax < getNerveStep()) {
        MR::startSystemSE(::sTimerSeSet[3]);
        return;
    }

    if (nerveStep % 0x3C != 0) {
        return;
    }

    if (nerveStep < frame - 0x258) {
        MR::startSystemSE(::sTimerSeSet[0]);
        return;
    }

    if (nerveStep < frame - 0x78) {
        MR::startSystemSE(::sTimerSeSet[1]);
        return;
    }

    if (nerveStep < frame) {
        MR::startSystemSE(::sTimerSeSet[2]);
        return;
    }
}

bool HipDropTimerSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        _D6 = true;
        return true;
    }

    if (MR::isMsgPlayerHipDropFloor(msg) && _98->mHitSensor == pReceiver && isNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvOff::sInstance)) {
        MR::invalidateClipping(this);
        setNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvSwitchDown::sInstance);
        _90->reset();
        return true;
    }

    return false;
}

bool HipDropTimerSwitch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgFloorTouch(msg) && _98->mHitSensor == pReceiver) {
        _D6 = true;
        return !_D5;
    }

    return false;
}

void HipDropTimerSwitch::exeOff() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::offSwitchA(this);
        MR::startBck(this, "Wait", nullptr);
        MR::startBrk(this, "Off");
        _90->reset();
    }

    // "Weak"
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        _D6 = true;
    }

    if (!_D5 && _D6) {
        _90->mVelocity += -10.0f;
        MR::startSound(this, "SE_OJ_PNC_KINOKO_BOUND");
    }
    _90->update();
}

void HipDropTimerSwitch::exeSwitchDown() {
    if (MR::isStep(this, 0)) {
        MR::startBck(this, "On", nullptr);
        MR::startBrk(this, "On");
    }

    if (MR::isGreaterStep(this, 0) && MR::isBckStopped(this)) {
        setNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvOn::sInstance);
    }
}

void HipDropTimerSwitch::exeOn() {
    if (MR::isFirstStep(this)) {
        MR::onSwitchA(this);
        MR::startSound(this, "SE_OJ_HIPDROP_SWITCH_ON");
        MR::shakeCameraNormal();
    }

    updateTimerSE();

    if (MR::isGreaterStep(this, mTimerMax)) {
        setNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvSwitchUp::sInstance);
    }
}

void HipDropTimerSwitch::exeSwitchUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Off", nullptr);
        MR::startBrk(this, "Off");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvOff::sInstance);
    }
}
