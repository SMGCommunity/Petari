#include "Game/MapObj/HipDropTimerSwitch.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SpringValue.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace {
    const char* const sTimerSeSet[2][4] = {{"SE_SY_TIMER_A_2", "SE_SY_TIMER_A_1", "SE_SY_TIMER_A_QUASI_0", "SE_SY_TIMER_A_0"},
                                           {"SE_SY_TIMER_B_2", "SE_SY_TIMER_B_1", "SE_SY_TIMER_B_QUASI_0", "SE_SY_TIMER_B_0"}};
};

namespace NrvHipDropSwitch {
    NEW_NERVE(HipDropTimerSwitchNrvOff, HipDropTimerSwitch, Off);
    NEW_NERVE(HipDropTimerSwitchNrvSwitchDown, HipDropTimerSwitch, SwitchDown);
    NEW_NERVE(HipDropTimerSwitchNrvOn, HipDropTimerSwitch, On);
    NEW_NERVE(HipDropTimerSwitchNrvSwitchUp, HipDropTimerSwitch, SwitchUp);
};  // namespace NrvHipDropSwitch

HipDropTimerSwitch::HipDropTimerSwitch(const char* pName)
    : LiveActor(pName), _8C(0), mMapObjConnector(nullptr), mCollisionParts(nullptr), mTimerMax(300), mTimerSFXType(0), mWasLightPressed(false),
      mIsLightPressed(false) {
    mSpring = new SpringValue;
    mMapObjConnector = new MapObjConnector(this);
    mSpring->setParam(0.0f, 0.0f, 0.2f, 0.9f, 0.0f);
    _9C.identity();
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
    MR::initCollisionParts(this, "HipDropTimerSwitch", getSensor("body"), nullptr);
    mCollisionParts = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("hit"), MR::CollisionScaleType_Unk2);
    MR::validateCollisionParts(mCollisionParts);
    initNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvOff::sInstance);
    MR::needStageSwitchWriteA(this, rIter);
    MR::tryRegisterDemoCast(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mTimerMax);
    MR::getJMapInfoArg1NoInit(rIter, &mTimerSFXType);

    if (mTimerSFXType <= 0) {
        mTimerSFXType = 0;
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
    mMapObjConnector->attachToUnder();
}

void HipDropTimerSwitch::control() {
    mWasLightPressed = mIsLightPressed;
    mIsLightPressed = false;
}

void HipDropTimerSwitch::calcAnim() {
    LiveActor::calcAnim();
    MtxPtr mtx = MR::getJointMtx(this, "Move");
    TPos3f mtx2;
    mtx2.setInline(mtx);
    f32 val = mSpring->mSpringValue;

    if (!MR::isNearZero(val)) {
        TVec3f pos, up;
        mtx2.getTrans(pos);
        MR::calcUpVec(&up, this);
        pos += up * val;
        mtx2.setTrans(pos);
        PSMTXCopy(mtx2, mtx);
    }

    mCollisionParts->setMtx(mtx2);
}

void HipDropTimerSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mMapObjConnector->connect();
}

void HipDropTimerSwitch::updateTimerSE() {
    if (MR::isPlayerDead()) {
        return;
    }

    if (MR::isPowerStarGetDemoActive()) {
        return;
    }

    s32 subSecond = mTimerMax % 60;
    s32 timeElapsed = getNerveStep() - subSecond;

    if (subSecond != 0) {
        timeElapsed += 60;
    }

    if (timeElapsed < 60) {
        return;
    }

    if (mTimerMax == getNerveStep()) {
        // Time's up
        MR::startSystemSE(::sTimerSeSet[mTimerSFXType][3]);
        return;
    }

    if (timeElapsed % 60 != 0) {
        return;
    }

    if (timeElapsed < mTimerMax - 600) {
        // > 10s remaining
        MR::startSystemSE(::sTimerSeSet[mTimerSFXType][0]);
    } else if (timeElapsed < mTimerMax - 120) {
        // 8 to 2s remaining
        MR::startSystemSE(::sTimerSeSet[mTimerSFXType][1]);
    } else if (timeElapsed < mTimerMax) {
        // < 2 seconds remaining
        MR::startSystemSE(::sTimerSeSet[mTimerSFXType][2]);
    }
}

bool HipDropTimerSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        mIsLightPressed = true;
        return true;
    }

    if (MR::isMsgPlayerHipDropFloor(msg) && mCollisionParts->mHitSensor == pReceiver &&
        isNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvOff::sInstance)) {
        MR::invalidateClipping(this);
        setNerve(&NrvHipDropSwitch::HipDropTimerSwitchNrvSwitchDown::sInstance);
        mSpring->reset();
        return true;
    }

    return false;
}

bool HipDropTimerSwitch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgFloorTouch(msg) && mCollisionParts->mHitSensor == pReceiver) {
        mIsLightPressed = true;
        return !mWasLightPressed;
    }

    return false;
}

void HipDropTimerSwitch::exeOff() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::offSwitchA(this);
        MR::startBck(this, "Wait", nullptr);
        MR::startBrk(this, "Off");
        mSpring->reset();
    }

    // "Weak"
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        mIsLightPressed = true;
    }

    if (!mWasLightPressed && mIsLightPressed) {
        mSpring->mVelocity += -10.0f;
        MR::startSound(this, "SE_OJ_PNC_KINOKO_BOUND");
    }

    mSpring->update();
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

HipDropTimerSwitch::~HipDropTimerSwitch() {
}
