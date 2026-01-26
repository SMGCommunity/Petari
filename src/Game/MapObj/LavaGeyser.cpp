#include "Game/MapObj/LavaGeyser.hpp"

#define DEG_TO_RAD_0_1 0.017453292f

namespace NrvLavaGeyser {
    NEW_NERVE(LavaGeyserNrvWaitSwitch, LavaGeyser, WaitSwitch);
    NEW_NERVE(LavaGeyserNrvWait, LavaGeyser, Wait);
    NEW_NERVE(LavaGeyserNerveSign, LavaGeyser, Sign);
    NEW_NERVE(LavaGeyserNerveShootUp, LavaGeyser, ShootUp);
    NEW_NERVE(LavaGeyserNerveShootKeep, LavaGeyser, ShootKeep);
    NEW_NERVE(LavaGeyserNerveShootDown, LavaGeyser, ShootDown);
}  // namespace NrvLavaGeyser

LavaGeyser::LavaGeyser(const char* pName)
    : LiveActor(pName), _8c(180), _90(180), _94(0.0f, 0.0f, 0.0f), _A0(0.0f, 1.0f, 0.0f), _B0(0.0f), _AC(0.0f), _B8(0.0f), _B4(0.0f) {
}  // last floats are loaded out of order
void LavaGeyser::init(const JMapInfoIter& iter) {
    MR::initDefaultPos(this, iter);
    // TODO: Fix register mismatch here
    TQuat4f quat1;
    f32 rotX = mRotation.x;
    f32 rotY = mRotation.y;
    f32 rotZ = mRotation.z;  // this matches worse without the seperate dec
    quat1.setEuler(DEG_TO_RAD_0_1 * rotX, DEG_TO_RAD_0_1 * rotY, DEG_TO_RAD_0_1 * rotZ);
    quat1.getYDir(_A0);
    MR::getJMapInfoArg0NoInit(iter, &_8c);
    MR::getJMapInfoArg1NoInit(iter, &_90);
    MR::useStageSwitchReadA(this, iter);
    initModelManagerWithAnm("LavaGeyser", nullptr, false);
    MR::startBtk(this, "LavaGeyser");
    MR::setBtkFrameAtRandom(this);
    MR::hideModel(this);
    MR::connectToSceneMapObj(this);
    initHitSensor(3);
    MR::addHitSensorCallbackMapObj(this, "attack", 1, mScale.y * 100.0f);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setGroupClipping(this, iter, 16);
    if (MR::isValidSwitchA(this)) {
        initNerve(&NrvLavaGeyser::LavaGeyserNrvWaitSwitch::sInstance);
    } else {
        initNerve(&NrvLavaGeyser::LavaGeyserNrvWait::sInstance);
    }
    makeActorAppeared();
}

void LavaGeyser::startClipped() {
    LiveActor::startClipped();
    MR::forceDeleteEffectAll(this);
    if (!isNerve(&NrvLavaGeyser::LavaGeyserNrvWaitSwitch::sInstance)) {
        setNerve(&NrvLavaGeyser::LavaGeyserNrvWait::sInstance);
    }
}

void LavaGeyser::updateHitSensor(HitSensor* pSensor) {
    TVec3f cPos = *MR::getPlayerCenterPos();
    cPos -= mPosition;
    f32 v1 = _A0.dot(cPos);
    TVec3f v2 = _94;
    v2 -= mPosition;
    f32 v3 = _A0.dot(v2) + -100.f;
    if (v3 < 0.f) {
        pSensor->mPosition.set(_94);
        return;
    }

    f32 v4 = 0.0f;
    if (v1 < 0.0f) {
        v4 = 0.0f;
    } else if (v1 > v3) {
        v4 = v3;
    } else {
        v4 = v1;
    }

    pSensor->mPosition.set(mPosition);
    TVec3f v5 = _A0;
    v5.mult(v4);
    pSensor->mPosition.addInLine(v5);
}

void LavaGeyser::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    MR::sendMsgEnemyAttackFireStrong(pReceiver, pSender);
}

void LavaGeyser::exeWaitSwitch() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
    }
    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvLavaGeyser::LavaGeyserNerveSign::sInstance);
    }
}

void LavaGeyser::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
        _94.set(mPosition);
    }
    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        setNerve(&NrvLavaGeyser::LavaGeyserNrvWaitSwitch::sInstance);
    } else if (MR::isStep(this, _8c)) {
        setNerve(&NrvLavaGeyser::LavaGeyserNerveSign::sInstance);
    }
}

void LavaGeyser::exeSign() {
    if (MR::isFirstStep(this)) {
        const TVec3f* pos = &mPosition;
        _94.set(*pos);
        MR::emitEffect(this, "Sign");  //
    }
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_GEYSER_SIGN", -1, -1, -1);
    if (MR::isStep(this, 90)) {
        MR::deleteEffect(this, "Sign");
        setNerve(&NrvLavaGeyser::LavaGeyserNerveShootUp::sInstance);
    }
}

void LavaGeyser::exeShootUp() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::validateHitSensors(this);
        MR::startSound(this, "SE_OJ_LAVA_GEYSER_SHOOT", -1, -1);
        MR::startBck(this, "LavaGeyserAppear", nullptr);
    }
    MR::copyJointPos(this, "Top", &_94);
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_GEYSER_SIGN", -1, -1, -1);
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_GEYSER_KEEP", -1, -1, -1);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvLavaGeyser::LavaGeyserNerveShootKeep::sInstance);
    }
}

void LavaGeyser::exeShootKeep() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "LavaGeyserWait", nullptr);
    }
    MR::copyJointPos(this, "Top", &_94);
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_GEYSER_SIGN", -1, -1, -1);
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_GEYSER_KEEP", -1, -1, -1);
    if (MR::isStep(this, _90)) {
        setNerve(&NrvLavaGeyser::LavaGeyserNerveShootDown::sInstance);
    }
}

void LavaGeyser::exeShootDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "LavaGeyserDisappear", nullptr);
    }
    MR::copyJointPos(this, "Top", &_94);
    MR::startLevelSound(this, "SE_OJ_LV_LAVA_GEYSER_KEEP", -1, -1, -1);
    if (MR::isBckStopped(this)) {
        MR::hideModel(this);
        setNerve(&NrvLavaGeyser::LavaGeyserNrvWait::sInstance);
    }
}
