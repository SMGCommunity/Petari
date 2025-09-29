#include "Game/MapObj/RevolvingWay.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"
#include <JSystem/JMath/JMath.hpp>

namespace NrvRevolvingWay {
    NEW_NERVE(RevolvingWayNrvWait, RevolvingWay, Wait);
};

RevolvingWay::~RevolvingWay() { }

RevolvingWay::RevolvingWay(const char* pName) : LiveActor(pName), _8C(0.0f, 1.0f) {
    _9C.x = 0.0f;
    _9C.y = 0.0f;
    _9C.z = 0.0f;
    _A8 = 500.0f;
}

void RevolvingWay::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("RevolvingWay", nullptr, false);
    MR::connectToSceneMapObj(this);
    MR::makeQuatFromRotate(&_8C, this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 0x10u, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "RevolvingWay", getSensor("body"), nullptr);
    MR::initStarPointerTarget(this, _A8, TVec3f(0.0f, 0.0f, 0.0f));
    initNerve(&NrvRevolvingWay::RevolvingWayNrvWait::sInstance);
    makeActorAppeared();
}

void RevolvingWay::control() { }

void RevolvingWay::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

void RevolvingWay::exeWait() {
    addAccelMoment();
    MR::rotateQuatMoment(&_8C, _9C);
    f32 v2;
    if (MR::testCorePadButtonB(0)) {
        v2 = 0.98f;
    } else {
        v2 = 0.9f;
    }
    _9C *= v2;
}

void RevolvingWay::addAccelMoment() {
    TVec3f stack_14;
    TVec3f stack_10;
    if (MR::isStarPointerPointing(this, 0, true, "弱") && MR::testCorePadButtonB(0)
    && MR::calcStarPointerStrokeRotateMoment(&stack_14, mPosition, _A8, 0)) {

    }
}
