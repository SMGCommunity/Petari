#include "Game/MapObj/PhantomShipBoxFloater.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/FloaterFloatingForceTypeSpring.hpp"
#include "Game/Util.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

namespace {
    const char* const cEffectName = "Ripple";
}  // namespace

namespace NrvPhantomShipBoxFloater {
    NEW_NERVE(HostTypeWait, PhantomShipBoxFloater, Wait);
    NEW_NERVE(HostTypeMove, PhantomShipBoxFloater, Move);
    NEW_NERVE(HostTypeMoveUnderWater, PhantomShipBoxFloater, MoveUnderWater);
}  // namespace NrvPhantomShipBoxFloater

PhantomShipBoxFloater::PhantomShipBoxFloater(const char* pName) : LiveActor(pName), mObjectName(), mFloaterOffset(gZeroVec), mRippleSeCounter() {
}

inline bool floaterIsMoving(FloaterFloatingForceTypeSpring* force) {
    return 0.1f < force->getCurrentVelocity().length();
}

void PhantomShipBoxFloater::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mFloaterOffset.set(mPosition);
    MR::getObjectName(&mObjectName, rIter);
    initModelManagerWithAnm(mObjectName, nullptr, false);
    MR::connectToSceneMapObj(this);
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostSRT(this, ::cEffectName, &mFloaterOffset, nullptr, nullptr);
    initSound(4, false);
    mRippleSeCounter = MR::getRandom(20l, 90l);
    initHitSensor(1);
    MR::initCollisionParts(this, mObjectName, MR::addBodyMessageSensorMapObj(this), nullptr);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    initNerve(GET_NERVE(PhantomShipBoxFloater, HostTypeWait));

    mFloatingForce = new FloaterFloatingForceTypeSpring(this, mObjectName);
    mFloatingForce->init(rIter);
    mFloatingForce->start();
    makeActorAppeared();
}

void PhantomShipBoxFloater::control() {
    mFloatingForce->movement();
    mFloatingForce->updateHostVelocity(&mVelocity);
}

f32 PhantomShipBoxFloater::getCurrentSinkDepth() const {
    TVec3f zDir, yDir;
    TPos3f depthMtx;
    depthMtx.setInline(getBaseMtx());
    depthMtx.getTrans(zDir);
    depthMtx.getYDir(yDir);
    f32 sign = MR::sign(yDir.dot(mFloaterOffset - zDir));
    return sign * mFloaterOffset.distance(zDir);
}

void PhantomShipBoxFloater::exeWait() {
    if (floaterIsMoving(mFloatingForce)) {
        setNerve(GET_NERVE(PhantomShipBoxFloater, HostTypeMove));
    }
}

void PhantomShipBoxFloater::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_BOX_FLOATER_BOUND");
        MR::emitEffect(this, ::cEffectName);
    }

    playRippleSe();

    if (MR::isOnPlayer(this)) {
        const char* objectName = mObjectName;
        f32 sinkDepth;

        if (MR::isEqualString("PhantomShipBoxFloaterA", objectName)) {
            sinkDepth = 580.0f;
        } else if (MR::isEqualString("PhantomShipBoxFloaterB", objectName)) {
            sinkDepth = 380.0f;
        } else if (MR::isEqualString("PhantomShipBoxFloaterC", objectName)) {
            sinkDepth = 180.0f;
        } else {
            sinkDepth = 0.0f;
        }

        if (sinkDepth < getCurrentSinkDepth()) {
            MR::deleteEffect(this, ::cEffectName);
            setNerve(GET_NERVE(PhantomShipBoxFloater, HostTypeMoveUnderWater));
        }
    } else {
        if (!floaterIsMoving(mFloatingForce)) {
            MR::deleteEffect(this, ::cEffectName);
            setNerve(GET_NERVE(PhantomShipBoxFloater, HostTypeWait));
        }
    }
}

void PhantomShipBoxFloater::exeMoveUnderWater() {
    if (!MR::isOnPlayer(this)) {
        const char* objectName = mObjectName;
        f32 sinkDepth;

        if (MR::isEqualString("PhantomShipBoxFloaterA", objectName)) {
            sinkDepth = 575.0f;
        } else if (MR::isEqualString("PhantomShipBoxFloaterB", objectName)) {
            sinkDepth = 375.0f;
        } else if (MR::isEqualString("PhantomShipBoxFloaterC", objectName)) {
            sinkDepth = 175.0f;
        } else {
            sinkDepth = 0.0f;
        }

        if (getCurrentSinkDepth() < sinkDepth) {
            setNerve(GET_NERVE(PhantomShipBoxFloater, HostTypeMove));
        }
    }
}

void PhantomShipBoxFloater::playRippleSe() {
    if (--mRippleSeCounter <= 0) {
        MR::startSound(this, "SE_OJ_BOX_FLOATER_RIPPLE");
        mRippleSeCounter = MR::getRandom(20l, 90l);
    }
}

PhantomShipBoxFloater::~PhantomShipBoxFloater() {
}
