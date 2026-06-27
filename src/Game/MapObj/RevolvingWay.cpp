#include "Game/MapObj/RevolvingWay.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "revolution/mtx.h"
#include <JSystem/JMath/JMath.hpp>

namespace NrvRevolvingWay {
    NEW_NERVE(RevolvingWayNrvWait, RevolvingWay, Wait);
};  // namespace NrvRevolvingWay

RevolvingWay::~RevolvingWay() {
}

RevolvingWay::RevolvingWay(const char* pName) : LiveActor(pName), _8C(0.0f, 1.0f) {
    mFriction.x = 0.0f;
    mFriction.y = 0.0f;
    mFriction.z = 0.0f;
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

void RevolvingWay::control() {
}

void RevolvingWay::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

void RevolvingWay::exeWait() {
    addAccelMoment();
    MR::rotateQuatMoment(&_8C, mFriction);
    f32 friction;
    if (MR::testCorePadButtonB(WPAD_CHAN0)) {
        friction = 0.98f;
    } else {
        friction = 0.9f;
    }
    mFriction *= friction;
}

void RevolvingWay::addAccelMoment() {
    // FIXME: broke this when adjusting some TVec stuff, fix later. (f regswap)
    // https://decomp.me/scratch/2QECD

    TVec3f stack_14;
    if (MR::isStarPointerPointing(this, 0, true, "弱") && MR::testCorePadButtonB(WPAD_CHAN0) &&
        MR::calcStarPointerStrokeRotateMoment(&stack_14, mPosition, _A8, 0)) {
        mFriction += stack_14.multiplyOperatorInline(0.04f);
        f32 mag = mFriction.length();
        if (mag > 0.15f) {
            mFriction *= (0.15f / mag);
        }
    }
}
