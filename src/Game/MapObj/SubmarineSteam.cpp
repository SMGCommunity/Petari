#include "Game/MapObj/SubmarineSteam.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvSubmarineSteam {
    NEW_NERVE(HostTypeWaitForSwitchOn, SubmarineSteam, WaitForSwitchOn);
    NEW_NERVE(HostTypeSteam, SubmarineSteam, Steam);
};  // namespace NrvSubmarineSteam

SubmarineSteam::SubmarineSteam(const char* pName) : LiveActor(pName), _8C(0.0f, 1.0f, 0.0f) {
}

void SubmarineSteam::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);
    initEffectKeeper(0, objName, false);
    initHitSensor(1);
    TVec3f offs(0.0f, 250.0f, 0.0f);
    HitSensor* sensor = MR::addHitSensorMapObj(this, "body", 8, 400.0f, offs);
    MR::setClippingTypeSphere(this, 800.0f, &sensor->mPosition);
    MR::setGroupClipping(this, rIter, 16);
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    MR::useStageSwitchReadA(this, rIter);
    initSound(4, false);

    if (MR::isValidSwitchA(this)) {
        initNerve(GET_NERVE(SubmarineSteam, HostTypeWaitForSwitchOn));
    } else {
        initNerve(GET_NERVE(SubmarineSteam, HostTypeSteam));
    }

    appear();
}

// todo -- nonmatching
void SubmarineSteam::initAfterPlacement() {
    TPos3f pos;
    pos.identity();
    pos.setRotate(mRotation * 0.0174);
    pos.getTrans(_8C);
    MR::normalize(&_8C);
}

// todo -- what
void SubmarineSteam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE(SubmarineSteam, HostTypeWaitForSwitchOn))) {
        if (MR::isSensorPlayer(pReceiver)) {
            TVec3f vec = _8C * 600.0f;
        }
    }
}

void SubmarineSteam::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void SubmarineSteam::endClipped() {
    LiveActor::endClipped();

    if (!isNerve(GET_NERVE(SubmarineSteam, HostTypeWaitForSwitchOn))) {
        MR::emitEffect(this, "Steam");
    }
}

void SubmarineSteam::exeSteam() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Steam");
    }

    if (MR::isEqualStageName("SkullSharkGalaxy")) {
        MR::startLevelSound(this, "SE_OJ_LV_MARINE_STEAM_OUT_S");
    } else {
        MR::startLevelSound(this, "SE_OJ_LV_MARINE_STEAM_OUT");
    }
}

void SubmarineSteam::exeWaitForSwitchOn() {
    if (MR::isOnSwitchA(this)) {
        setNerve(GET_NERVE(SubmarineSteam, HostTypeSteam));
    }
}

SubmarineSteam::~SubmarineSteam() {
}
