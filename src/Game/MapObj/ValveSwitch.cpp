#include "Game/MapObj/ValveSwitch.hpp"

ValveSwitch::ValveSwitch(const char *pName) : LiveActor(pName) {
    _8C = nullptr;
    mObjConnector = nullptr;
    _C4 = false;
    
    mObjConnector = new MapObjConnector(this);
    _90.identity();
}

void ValveSwitch::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::needStageSwitchWriteA(this, rIter);
    initModelManagerWithAnm("ValveSwitch", nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    TVec3f binder;
    binder.scale(-75.0f, mGravity);
    MR::addHitSensorAtJoint(this, "binder", "Valve", 97, 8, 150.0f, binder);
    _90.set(MR::getJointMtx(this, "Valve"));
    MR::initCollisionParts(this, "ValveCol", getSensor("binder"), _90.toMtxPtr());
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::getJMapInfoArg0NoInit(rIter, &_C4);
    initNerve(&NrvValveSwitch::ValveSwitchNrvWait::sInstance);
    makeActorAppeared();
}

void ValveSwitch::initAfterPlacement() {
    mObjConnector->attachToUnder();
}

void ValveSwitch::control() {
    _90.set(MR::getJointMtx(this, "Valve"));
    MR::makeMtxWithoutScale(&_90, _90);
}

void ValveSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mObjConnector->connect();
}

void ValveSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "ValveSwitchOn");
        MR::setBrkFrameAndStop(this, 0.0f);

        if (_C4) {
            MR::startBck(this, "ValveSwitchReverseOn", nullptr);
        }
        else {
            MR::startBck(this, "ValveSwitchOn", nullptr);
        }

        MR::setBckFrameAndStop(this, 0.0f);
    }
}

void ValveSwitch::exeAdjust() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "ValveSwitchOn");
        MR::setBrkFrameAndStop(this, 0.0f);
    }

    f32 step = getNerveStep() / 3.0f;
    HitSensor* sensor = getSensor("body");
    JMAVECLerp(&_8C->mPosition, &sensor->mPosition, &_8C->mPosition, step);

    if (MR::isStep(this, 3)) {
        setNerve(&NrvValveSwitch::ValveSwitchNrvValve::sInstance);
    }
}

void ValveSwitch::exeValve() {
    if (MR::isFirstStep(this)) {
        if (_C4) {
            MR::startBck(this, "ValveSwitchReverseOn", nullptr);
        }
        else {
            MR::startBck(this, "ValveSwitchOn", nullptr);
        }

        MR::startBrk(this, "ValveSwitchOn");
        MR::startBckPlayer("ScrewSwitchOn", (s32)0);
    }

    if (MR::isLessStep(this, 30)) {
        MR::startLevelSound(this, "SE_OJ_LV_VALVE_SWITCH_MOVE", -1, -1, -1);
    }

    if (MR::isStep(this, 30)) {
        MR::startSound(this, "SE_OJ_VALVE_SWITCH_ON", -1, -1);
        MR::tryRumblePadVeryStrong(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isBckStopped(this)) {
        MR::onSwitchA(this);
        setNerve(&NrvValveSwitch::ValveSwitchNrvEnd::sInstance);
    }
}

void ValveSwitch::exeEnd() {
    if (MR::isStep(this, 0)) {
        MR::invalidateHitSensors(this);
        if (MR::isPlayerInRush()) {
            MR::endBindAndPlayerWait(this);
            _8C = nullptr;
        }
    }
}

// ValveSwitch::receiveOtherMsg

void ValveSwitch::updateBindActorMtx() {
    TPos3f posMtx;
    posMtx.set(_8C->getBaseMtx());
    HitSensor* sensor = getSensor("binder");
    posMtx.mMtx[0][3] = sensor->mPosition.x;
    posMtx.mMtx[1][3] = sensor->mPosition.y;
    posMtx.mMtx[2][3] = sensor->mPosition.z;
    MR::setBaseTRMtx(_8C, posMtx);
}

namespace NrvValveSwitch {
    INIT_NERVE(ValveSwitchNrvWait);
    INIT_NERVE(ValveSwitchNrvAdjust);
    INIT_NERVE(ValveSwitchNrvValve);
    INIT_NERVE(ValveSwitchNrvEnd);
};

ValveSwitch::~ValveSwitch() {
    
}
