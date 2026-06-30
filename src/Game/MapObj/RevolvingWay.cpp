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

RevolvingWay::RevolvingWay(const char* pName) : LiveActor(pName), mRotateQuat(0.0f, 1.0f) {
    mFriction.x = 0.0f;
    mFriction.y = 0.0f;
    mFriction.z = 0.0f;
    mRadius = 500.0f;
}

void RevolvingWay::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("RevolvingWay", nullptr, false);
    MR::connectToSceneMapObj(this);
    MR::makeQuatFromRotate(&mRotateQuat, this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 0x10u, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "RevolvingWay", getSensor("body"), nullptr);
    MR::initStarPointerTarget(this, mRadius, TVec3f(0.0f, 0.0f, 0.0f));
    initNerve(&NrvRevolvingWay::RevolvingWayNrvWait::sInstance);
    makeActorAppeared();
}

void RevolvingWay::control() {
}

void RevolvingWay::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mRotateQuat);
}

void RevolvingWay::exeWait() {
    addAccelMoment();
    MR::rotateQuatMoment(&mRotateQuat, mFriction);
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

    TVec3f rotateMoment;
    if (MR::isStarPointerPointing(this, 0, true, "弱") && MR::testCorePadButtonB(WPAD_CHAN0) &&
        MR::calcStarPointerStrokeRotateMoment(&rotateMoment, mPosition, mRadius, 0)) {
        mFriction += rotateMoment.multiplyOperatorInline(0.04f);
        f32 mag = mFriction.length();
        if (mag > 0.15f) {
            mFriction *= (0.15f / mag);
        }
    }
}
