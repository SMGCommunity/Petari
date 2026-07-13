#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

void MarioActor::setupSensors() {
    initHitSensor(4);

    MR::addHitSensorCallback(this, "body", ATYPE_PLAYER, 32, 100.0f);
    MR::addHitSensorCallback(this, "ex-eye", ATYPE_PUSH, 32, 100.0f);
    MR::addHitSensorCallback(this, "eye", ATYPE_EYE, 64, 2000.0f);

    MR::addHitSensor(this, "dummy", ATYPE_PLAYER, 1, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));

    getSensor("dummy")->invalidate();

    _468 = 0;
    _46C = nullptr;

    _7DC = 0;

    for (s32 idx = 0; idx < ARRAY_SIZE(_4D0); idx++) {
        _4D0[idx] = nullptr;
    }

    for (s32 idx = 0; idx < ARRAY_SIZE(_428); idx++) {
        _428[idx] = nullptr;
    }

    initScouter();

    _3E5 = false;
    _3E8 = false;

    MR::initStarPointerTarget(this, 80.0f, TVec3f(0.0f, 80.0f, 0.0f));
}

// void MarioActor::updateHitSensor(HitSensor*) {}

// void MarioActor::doTrampleJump(HitSensor*) {}

// void MarioActor::trampleJump(f32, f32) {}

//void MarioActor::attackSensor(HitSensor*, HitSensor*) {}

bool MarioActor::sendMsgToSensor(HitSensor* pSensor, u32 msg) {
    return pSensor->receiveMessage(msg, getSensor("body"));
}

void MarioActor::resetSensorCount() {
    _930 = 0;
    _7DC = 0;
    _46C = 0;
}

// void MarioActor::recordScoutingObject(HitSensor*) {}

// void MarioActor::updateScouter() {}

void MarioActor::initScouter() {
    _9CC = 0.0f;
    _9D0 = 60.0f;
    _9D4 = 0;
    _9D8.set2(0.0f);

    getSensor("ex-eye")->mRadius = 100.0f;
    getSensor("ex-eye")->validate();
}

void MarioActor::initForJump() {
    _988 = 0;
    _989 = 0;
}
